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
#include<../EBU/EBUManager.h>

using namespace std;
/*
include the extended socket to Ground control and regular socket to the
simulator.
*/



/*Needs to get the data out, make some calls to a mapping object
 * and forward everything to the right EBU and the right pin with
 * the right values.
 */

int main()
{
	EBUManager ebuMan = EBUManager();
	extendedSocket exSock = extendedSocket();
	int c;
	while((c=getchar()) != EOF)
	{
		usleep(10005);   //1000 microseconds in a millisecond.
		packet pkt = extendedSocket.getPacket();
		if ((ebuMan.sendCommand(pkt.pin, pkt.value))<0){
			perror("error with sending command");
			return 0;
		}

	}
	return 0;
}


