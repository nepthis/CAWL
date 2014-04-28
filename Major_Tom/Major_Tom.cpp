#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include <queue>

#include <../Net/ExtendedSocket.h>
#include<../EBU/EBUManager.h>
#include <../Packets/CawlPacket.h>
#include <../Packets/EBUPacketIn.h>
#include <mutex>

using namespace std;
/*
include the extended socket to Ground control and regular socket to the
simulator.
*/

/*Needs to get the data out, make some calls to a mapping object
 * and forward everything to the right EBU and the right pin with
 * the right values.
 */


/*
 * Ny lösning, skapa tråd för att ta emot paket till en buffer och en annan tråd för att skicka
 * från buffern. EVERYBODY WINS. behöver mest troligt en buffer för inkommmande data och en
 * för utgående data till/från EBU
 */
void sendToEBU(queue<EBUPacketIn*> q, EBUManager man){
	EBUPacketIn p;
	while(1){
		//if queue is empty send full stop, this is more of a failsafe for now.
		if (q.empty()){
			//send stop
		}
		p = q.pop();
				man.sendCommand(p);
	}

}

/*
 * The main function will run threads for receiving packets from the other
 * gateway and another thread that empties the buffer and send packets to the EBU.
 * TODO: investigates if threads are needed to "read" data from the EBU and more
 * queues.
 */
int main()
{
	EBUManager ebuMan = EBUManager();
	CawlPacket *pkt;
	queue<EBUPacketIn*> fromEBU;
	queue<EBUPacketOut*> toEBU;
	//int gatewaySocket = ExtendedSocket();
	int quit = 0;

	//gemensam quit-variabel för att stänga av snyggt
	if((ebuMan.startConnection(1,"10.10.0.1")) < 0){
		perror("Connection to EBU one has failed, terminating");
		quit = 1;
		return 0;
	}

	while(1)
	{
		usleep(10005);   //1000 microseconds in a millisecond.
		&pkt = extendedSocket.getPacket();


	}
	return 0;
}


