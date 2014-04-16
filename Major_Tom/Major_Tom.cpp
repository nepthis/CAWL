#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <../Net/ExtendedSocket.h>
using namespace std;
/*
include the extended socket to Ground control and regular socket to the
simulator.
*/

double convertVoltToBit(int voltIn)
{
	double voltOut = voltIn;

	return voltOut;
}



/*Needs to get the data out, make some calls to a mapping object
 * and forward everything to the right EBU and the right pin with
 * the right values.
 */
void forwardPacket(packet pkt, socket skt){
	string str1 = pkt.get();

}
int main()
{
	//Need to fix proper code here
	socket ebuSocket = socket(0,0,0);
	extendedSocket exSock = extendedSocket();
	int c;
	while((c=getchar()) != EOF)
	{
		usleep(10000);   //1000 microseconds in a millisecond.
		packet pkt = extendedSocket.getPacket();
		forwardPacket(pkt, ebuSocket);

	}
	return 0;
}


