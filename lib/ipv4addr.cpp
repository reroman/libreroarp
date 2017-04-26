#include <reroman/ipv4addr.hpp>

#include <cerrno>
#include <cstring>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

using namespace std;
using namespace reroman;

IPv4Addr::IPv4Addr( uint32_t addr ) noexcept
: data( {addr} ){}

IPv4Addr::IPv4Addr( string addr )
{
	setAddr( addr );
}

IPv4Addr::IPv4Addr( struct in_addr addr ) noexcept
: data( addr ){}

IPv4Addr::IPv4Addr( const IPv4Addr &addr ) noexcept
: data( addr.data ){}

string IPv4Addr::toString( void ) const
{
	char tmp[INET_ADDRSTRLEN];

	inet_ntop( AF_INET, &data, tmp, INET_ADDRSTRLEN );
	return string( tmp );
}

bool IPv4Addr::isValidNetmask( void ) const noexcept
{
	uint32_t a = ntohl( data.s_addr );

	if( !a || (a & 0xff) >= 254 )
		return false;
	a = ~a;
	uint32_t b = a + 1;
	return !( b & a );
}

void IPv4Addr::setAddr( string addr )
{
	if( !inet_aton( addr.c_str(), &data ) )
		throw invalid_argument( addr + " is not a valid IPv4 address" );
}

IPv4Addr IPv4Addr::operator+( int n ) const
{
	int64_t tmp = ntohl( data.s_addr );
	tmp += n;

	if( tmp > 0xffffffff )
		throw overflow_error( "IPv4 overflow" );
	if( tmp < 0 )
		throw underflow_error( "IPv4 underflow" );

	return IPv4Addr( htonl( static_cast<uint32_t>(tmp) ) );
}

IPv4Addr IPv4Addr::operator-( int n ) const
{
	int64_t tmp = ntohl( data.s_addr );
	tmp -= n;

	if( tmp > 0xffffffff )
		throw overflow_error( "IPv4 overflow" );
	if( tmp < 0 )
		throw underflow_error( "IPv4 underflow" );

	return IPv4Addr( htonl( static_cast<uint32_t>(tmp) ) );
}

IPv4Addr& IPv4Addr::operator++( void )
{
	int64_t tmp = ntohl( data.s_addr );
	tmp++;

	if( tmp > 0xffffffff )
		throw overflow_error( "IPv4 overflow" );

	data.s_addr = htonl( tmp );
	return *this;
}

IPv4Addr IPv4Addr::operator++( int )
{
	IPv4Addr res( *this );
	int64_t tmp = ntohl( data.s_addr );
	tmp++;

	if( tmp > 0xffffffff )
		throw overflow_error( "IPv4 overflow" );

	data.s_addr = htonl( tmp );
	return res;
}

IPv4Addr& IPv4Addr::operator--( void )
{
	int64_t tmp = ntohl( data.s_addr );
	tmp--;

	if( tmp < 0 )
		throw underflow_error( "IPv4 underflow" );

	data.s_addr = htonl( tmp );
	return *this;
}

IPv4Addr IPv4Addr::operator--( int )
{
	IPv4Addr res( *this );
	int64_t tmp = ntohl( data.s_addr );
	tmp--;

	if( tmp < 0 )
		throw underflow_error( "IPv4 underflow" );

	data.s_addr = htonl( tmp );
	return res;
}

IPv4Addr IPv4Addr::getFromInterface( string ifname )
{
	int sockfd;
	struct ifreq nic;

	if( (sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
		throw system_error( errno, generic_category(), "socket" );

	if( ifname.size() >= IFNAMSIZ )
		ifname.resize( IFNAMSIZ - 1 );
	strcpy( nic.ifr_name, ifname.c_str() );

	if( ioctl( sockfd, SIOCGIFADDR, &nic ) < 0 ){
		close( sockfd );
		throw system_error( errno, generic_category(), ifname );
	}

	IPv4Addr result( ((sockaddr_in*)&nic.ifr_addr)->sin_addr );
	close( sockfd );
	return result;
}

IPv4Addr IPv4Addr::getNmaskFromInterface( string ifname )
{
	int sockfd;
	struct ifreq nic;

	if( (sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
		throw system_error( errno, generic_category(), "socket" );

	if( ifname.size() >= IFNAMSIZ )
		ifname.resize( IFNAMSIZ - 1 );
	strcpy( nic.ifr_name, ifname.c_str() );

	if( ioctl( sockfd, SIOCGIFNETMASK, &nic ) < 0 ){
		close( sockfd );
		throw system_error( errno, generic_category(), ifname );
	}

	IPv4Addr result( ((sockaddr_in*)&nic.ifr_netmask)->sin_addr );
	close( sockfd );
	return result;
}

