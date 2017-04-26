#include <reroman/hwaddr.hpp>
#include <sstream>
#include <vector>
#include <boost/algorithm/string/split.hpp>

#include <cstring>
#include <cerrno>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>

#include <config.hpp>
#ifdef ENABLE_VENDORS
#include <sqlite3.h>
#define DBVENDORS	"vendors.db"
#else
# warning "VENDORS_DISABLED"
#endif

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


string HwAddr::getVendor( void ) const
{
	string vendor( "Unknown" );
#ifdef ENABLE_VENDORS
	sqlite3 *db;
	ostringstream sql;

	if( sqlite3_open_v2( DBVENDORS, &db,
				SQLITE_OPEN_READONLY, NULL ) != SQLITE_OK )
		return vendor;

	sql << "SELECT vendor FROM Vendors WHERE mac='"
		<< uppercase << hex;
	for( int i = 0 ; i < HwAddrLen / 2 ; i++ ){
		sql.fill( '0' );
		sql.width( 2 );
		sql << static_cast<int>( data[i] );
	}
	sql << "' LIMIT 1";

	sqlite3_exec( db, sql.str().c_str(), 
			[]( void *strvendor, int, char **value, char** ) -> int{
			static_cast<string*>( strvendor )->operator=( *value );
			return 0;
			}, &vendor, NULL );
	sqlite3_close( db );
#endif
	return vendor;
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

