#include <reroman/hwaddr.hpp>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <system_error>
#include <boost/algorithm/string/split.hpp>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

using namespace std;
using namespace reroman;

HwAddr::HwAddr( void ) noexcept
{
	clear();
}

HwAddr::HwAddr( string addr )
{
	setData( addr );
}

HwAddr::HwAddr( initializer_list<uint8_t> bytes )
{
	setData( bytes );
}

HwAddr::HwAddr( const struct ether_addr *addr )
{
	setData( addr );
}

HwAddr::HwAddr( const uint8_t *bytes )
{
	setData( bytes );
}

HwAddr::HwAddr( const HwAddr &addr ) noexcept
{
	for( int i = 0 ; i < HwAddrLen ; i++ )
		data[i] = addr.data[i];
}

bool HwAddr::isNull( void ) const noexcept
{
	for( auto &i : data )
		if( i )
			return false;
	return true;
}

string HwAddr::toString( void ) const
{
	ostringstream out;

	out << hex;
	for( int i = 0 ; i < HwAddrLen ; ){
		out.fill( '0' );
		out.width( 2 );
		out << static_cast<int>( data[i] );
		if( ++i != HwAddrLen )
			out << ':';
	}
	return out.str();
}

void HwAddr::setData( string addr )
{
	string::size_type noNum;
	string errorMessage = addr + " is not a valid MAC address";
	vector<string> toks;
	boost::split( toks, addr, [](char c){ return c == ':'; } );

	if( toks.size() != HwAddrLen )
		throw invalid_argument( errorMessage );

	try{
		for( int i = 0 ; i < HwAddrLen ; i++ ){
			int aux = stoi( toks[i], &noNum, 16 );
			if( noNum < toks[i].size() || aux > 0xff )
				throw invalid_argument( errorMessage );
			data[i] = static_cast<uint8_t>( aux );
		}
	}
	catch( out_of_range ){
		throw invalid_argument( errorMessage );
	}
}

void HwAddr::setData( initializer_list<uint8_t> bytes )
{
	if( bytes.size() < HwAddrLen )
		throw invalid_argument( "The given list is not a valid MAC address" );

	auto i = bytes.begin();
	for( auto &j : data ){
		j = *i;
		i++;
	}
}

HwAddr HwAddr::getFromInterface( string ifname )
{
	int sockfd;
	struct ifreq nic;

	if( (sockfd = socket( AF_INET, SOCK_DGRAM, 0 )) < 0 )
		throw system_error( errno, generic_category(), "socket" );

	if( ifname.size() >= IFNAMSIZ )
		ifname.resize( IFNAMSIZ - 1 );
	strcpy( nic.ifr_name, ifname.c_str() );

	if( ioctl( sockfd, SIOCGIFHWADDR, &nic ) < 0 ){
		close( sockfd );
		throw system_error( errno, generic_category(), ifname );
	}

	HwAddr result( reinterpret_cast<uint8_t*>(nic.ifr_hwaddr.sa_data) );
	close( sockfd );
	return result;
}

