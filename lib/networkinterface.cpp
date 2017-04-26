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
	throw( system_error )
	: name( ifname ),
	  index( 0 )
{
	struct ifreq nic;
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sfd < 0 )
		throw system_error( errno, generic_category(), "socket" );

	if( name.size() >= IFNAMSIZ )
		name.resize( IFNAMSIZ - 1 );

	strcpy( nic.ifr_name, name.c_str() );
	if( ioctl( sfd, SIOCGIFINDEX, &nic ) < 0 ){
		close( sfd );
		throw system_error( errno, generic_category(), name );
	}
	close( sfd );
	index = nic.ifr_ifindex;
}

NetworkInterface::NetworkInterface( int index )
	throw( system_error )
	: name( "" ),
	index( index )
{
	struct ifreq nic;
	int sfd = socket( AF_INET, SOCK_DGRAM, 0 );

	if( sfd < 0 )
		throw system_error( errno, generic_category(), "socket" );

	nic.ifr_ifindex = index;
	if( ioctl( sfd, SIOCGIFNAME, &nic ) < 0 ){
		close( sfd );
		throw system_error( errno, generic_category(), "invalid index" );
	}
	close( sfd );
	name = nic.ifr_name;
}

