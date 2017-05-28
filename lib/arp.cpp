#include <reroman/arp/arp.hpp>
#include <system_error>

#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_arp.h>
#include <net/ethernet.h>
#include <sys/ioctl.h>

using namespace std;
using namespace reroman;
using namespace reroman::arp;

namespace reroman{
	namespace arp{
		bool addStaticSystemEntry( const reroman::NetworkInterface &nic,
				const reroman::IPv4Addr &ip, const reroman::HwAddr &hw )
		{
			if( !nic.isBinded() ){
				errno = EINVAL;
				return false;
			}

			struct arpreq arp;
			int sock = socket( AF_INET, SOCK_DGRAM, 0 );

			arp.arp_pa.sa_family = AF_INET;
			memcpy( arp.arp_pa.sa_data + 2, &ip.getInAddr(),
					IPv4Addr::IPv4AddrLen );
			arp.arp_ha.sa_family = ARPHRD_ETHER;
			hw.copyTo( reinterpret_cast<uint8_t*>(arp.arp_ha.sa_data) );
			strcpy( arp.arp_dev, nic.getName().c_str() );
			arp.arp_flags = ATF_COM | ATF_PERM;

			if( ioctl( sock, SIOCSARP, &arp ) == -1 ){
				close( sock );
				return false;
			}
			close( sock );
			return true;
		}

		bool delSystemEntry( const reroman::NetworkInterface &nic,
				const reroman::IPv4Addr &ip )
		{
			if( !nic.isBinded() ){
				errno = EINVAL;
				return false;
			}

			struct arpreq arp;
			int sock = socket( AF_INET, SOCK_DGRAM, 0 );

			arp.arp_pa.sa_family = AF_INET;
			memcpy( arp.arp_pa.sa_data + 2, &ip.getInAddr(),
					IPv4Addr::IPv4AddrLen );
			strcpy( arp.arp_dev, nic.getName().c_str() );
			arp.arp_flags = 0;

			if( ioctl( sock, SIOCDARP, &arp ) == -1 ){
				close( sock );
				return false;
			}
			close( sock );
			return true;
		}

		reroman::HwAddr getSystemEntry( const reroman::NetworkInterface &nic,
				const reroman::IPv4Addr &ip )
		{
			if( !nic.isBinded() )
				throw invalid_argument( "Invalid network interface" );

			struct arpreq arp;
			int sock = socket( AF_INET, SOCK_DGRAM, 0 );

			arp.arp_pa.sa_family = AF_INET;
			memcpy( arp.arp_pa.sa_data + 2, &ip.getInAddr(),
					IPv4Addr::IPv4AddrLen );
			strcpy( arp.arp_dev, nic.getName().c_str() );
			arp.arp_flags = 0;

			if( ioctl( sock, SIOCGARP, &arp ) == -1 ){
				close( sock );
				if( errno == ENXIO )
					throw out_of_range( ip.toString() + " not found in the ARP cache" );
				throw system_error( errno, generic_category(), "getSystemEntry" );
			}
			close( sock );
			return reroman::HwAddr( reinterpret_cast<uint8_t*>(arp.arp_ha.sa_data) );
		}
	}
}

ARPSocket::ARPSocket( unsigned int msecs )
{
	sock = socket( AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP) );

	if( sock < 0 )
		throw system_error( errno, generic_category(), "ARPSocket" );
	if( !setTimeout( msecs ) ){
		close( sock );
		throw system_error( errno, generic_category(), "ARPSocket" );
	}
}

ARPSocket::~ARPSocket()
{
	if( sock > 0 )
		close( sock );
}

bool ARPSocket::setTimeout( unsigned int msecs )
{
	struct timeval aux;

	aux.tv_sec = msecs / 1000;
	aux.tv_usec = msecs % 1000 * 1000;
	if( setsockopt( sock, SOL_SOCKET, SO_RCVTIMEO,
			&aux, sizeof(timer) ) < 0 )
		return false;
	timer = aux;
	return true;
}

ARPSocket& ARPSocket::operator=( ARPSocket && sock )
{
	close( this->sock );
	this->sock = sock.sock;
	sock.sock = -1;
	return *this;
}

bool ARPSocket::receive( ARPFrame &frame, HwAddr *sender )
{
	struct sockaddr_ll sll{ 0, 0, 0, 0, 0, 0, 0 };
	socklen_t size = sizeof(sll);

	if( recvfrom( sock, &frame, sizeof(ARPFrame), 0,
				(sockaddr*) &sll, &size ) <= 0 ){
		if( errno == EAGAIN )
			return false;
		else
			throw system_error( errno, generic_category(),
					"ARPSocket::receive" );
	}
	if( sender )
		sender->setData( sll.sll_addr );
	return true;
}

bool ARPSocket::send( const ARPFrame &frame, const HwAddr &dst,
	   const NetworkInterface &nic )
{
	struct sockaddr_ll sll{ AF_PACKET,
		htons( ETH_P_ARP ),
		nic.getIndex(),
		0, 0, HwAddr::HwAddrLen, 0 };
	dst.copyTo( sll.sll_addr );

	return sendto( sock, &frame, sizeof(ARPFrame), 0,
				(sockaddr*) &sll, sizeof(sll) ) > 0;
}

bool ARPSocket::bind( const NetworkInterface &nic )
{
	struct sockaddr_ll sll{ AF_PACKET,
		htons( ETH_P_ARP ),
		nic.getIndex(),
		0, 0, HwAddr::HwAddrLen, 0 };
	nic.getHwAddress().copyTo( sll.sll_addr );

	return !::bind( sock, (sockaddr*) &sll, sizeof(sll) );
}

bool ARPSocket::resolve( const IPv4Addr &ip,
		const NetworkInterface &nic, HwAddr *result )
{
	ARPFrame frame;
	const HwAddr broadcast{ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

	frame.setSourceHwAddr( nic.getHwAddress() );
	frame.ipSrc = nic.getAddress().toNetworkInt();
	frame.ipTgt = ip.toNetworkInt();

	if( !send( frame, broadcast, nic ) )
		return false;

	if( receive( frame ) && 
			frame.getOpCode() == OperationCode::REPLY &&
			frame.ipSrc == ip.toNetworkInt() ){
		if( result )
			result->setData( frame.hwSrc );
		return true;
	}
	return false;
}

