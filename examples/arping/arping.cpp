#include <iostream>
#include <reroman/arp/arp.hpp>
using namespace std;
using namespace reroman;

int main( int argc, char **argv )
{
	if( argc < 3 ){
		cerr << "Uso: " << *argv << " <interface> <ip>\n";
		return -1;
	}

	try{
		NetworkInterface nic( argv[1] );
		IPv4Addr host( argv[2] );
		arp::ARPSocket socket;
		HwAddr result;

		if( socket.resolve( host, nic, &result ) )
			cout << "IP:\t" << host
				<< "\nHw:\t" << result.toString()
				<< endl;
		else
			cout << "Couldn't resolve " << host << endl;
	}
	catch( exception &e ){
		cerr << e.what() << endl;
		return -1;
	}
	return 0;
}
