#include <iostream>
#include <csignal>
#include <reroman/arp/arp.hpp>
using namespace std;
using namespace reroman;
using namespace reroman::arp;

int main( int argc, char **argv )
{
	if( argc < 2 ){
		cerr << "Use: " << *argv << " <interface>" << endl;
		return -1;
	}
	try{
		NetworkInterface nic( argv[1] );
		ARPSocket sock( 200 );
		int hostsUp = 0;

		auto ipLocal = nic.getAddress();
		auto mask = nic.getNetmask();
		auto request = IPv4Addr::makeNetAddress( ipLocal, mask ) + 1;
		auto broad = IPv4Addr::makeBroadcast( ipLocal, mask );

		for(  ; request < broad ; request++ ){
			cout << "Resolving " << request << '\r' << flush;
			if( sock.resolve( request, nic ) ){
				hostsUp++;
				cout << request << " is up         " << endl;
			}
		}
		cout << hostsUp << " hosts up              " << endl;
		return 0;
	}
	catch( system_error &e ){
		cerr << e.what() << endl;
	}
}
