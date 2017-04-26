#include <reroman/networkinterface.hpp>
using namespace std;
using namespace reroman;

int main()
{
	try{
		for( int i = 1 ; ; i++ ){
			NetworkInterface nic( i );
			cout << i << ") " << nic.getName()
				<< "\n   Hw Address:\t" << nic.getHwAddress();

			// Intenta obtener direcciones IP
			try{
				auto ip = nic.getAddress();
				auto mask = nic.getNetmask();

				cout << "\n   Network:\t" << IPv4Addr::makeNetAddress( ip, mask )
					<< "\n   Netmask:\t" << mask
					<< "\n   IP Address:\t" << ip
					<< "\n   Broadcast:\t" << IPv4Addr::makeBroadcast( ip, mask )
					<< '\n' <<endl;
			}
			catch( system_error& ){
				cout << "\n\n";
			}
		}
	}
	catch( system_error & ){ // Ya no hay más dispositivos
		return 0;
	}
}
