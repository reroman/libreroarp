#include <reroman/networkinterface.hpp>

#include <cerrno>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <unistd.h>

using namespace std;
using namespace reroman;


NetworkInterface::NetworkInterface( string ifname )
{
	if( !bind( ifname ) )
		throw system_error( errno, generic_category(), 
				"NetworkInterface(" + ifname + ")" );
}

NetworkInterface::NetworkInterface( int index )
{
	if( !bind( index ) )
		throw system_error( errno, generic_category(),
				"NetworkInterface(" + to_string(index) + ")" );
}

bool NetworkInterface::isPromiscModeEnabled( void ) const
{
	struct ifreq nic;
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sfd < 0 )
		throw system_error( errno, generic_category(), "socket" );

	strcpy( nic.ifr_name, name.c_str() );
	if( ioctl( sfd, SIOCGIFFLAGS, &nic ) < 0 ){
		close( sfd );
		throw system_error( errno, generic_category(), name );
	}

	close( sfd );
	return nic.ifr_flags & IFF_PROMISC;
}

bool NetworkInterface::setPromiscMode( bool value )
{
	struct ifreq nic;
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sfd < 0 )
		return false;

	strcpy( nic.ifr_name, name.c_str() );
	if( ioctl( sfd, SIOCGIFFLAGS, &nic ) < 0 ){
		close( sfd );
		return false;
	}

	if( value )
		nic.ifr_flags |= IFF_PROMISC;
	else
		nic.ifr_flags &= ~IFF_PROMISC;

	if( ioctl( sfd, SIOCSIFFLAGS, &nic ) < 0 ){
		close( sfd );
		return false;
	}
	close( sfd );
	return true;
}

bool NetworkInterface::bind( string ifname )
{
	struct ifreq nic;
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sfd < 0 )
		return false;

	if( ifname.size() >= IFNAMSIZ )
		ifname.resize( IFNAMSIZ - 1 );

	strcpy( nic.ifr_name, ifname.c_str() );
	if( ioctl( sfd, SIOCGIFINDEX, &nic ) < 0 ){
		close( sfd );
		return false;
	}
	close( sfd );
	name = ifname;
	index = nic.ifr_ifindex;
	binded = true;
	return true;
}

bool NetworkInterface::bind( int index )
{
	struct ifreq nic;
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sfd < 0 )
		return false;

	nic.ifr_ifindex = index;
	if( ioctl( sfd, SIOCGIFNAME, &nic ) < 0 ){
		close( sfd );
		return false;
	}
	close( sfd );
	name = nic.ifr_name;
	this->index = index;
	binded = true;
	return true;
}
