//#include <stdio.h>
//#include <ctype.h>
//#include <stdlib.h>
//#include <unistd.h>
#include <string>
//#include <sys/types.h>
#include <signal.h>
#include <stdint.h>

#include <netinet/in.h>
#include <netdb.h> 

//For threads and mutex
#include <pthread.h>
#include <queue>

#include <../Netapi/CawlSocket.h>
#include<../EBU/EBUManager.h>
#include <../Packets/CawlPacket.h>
#include <../Packets/EBUPacketAnalogIn.h>
#include <../Packets/EBUPacketAnalogOut.h>

using namespace std;
//Globals
int timeToQuit = 0;

Packets::EBUPacketAnalogIn packetAnalogIn;

EBU::EBUManager ebuMan = EBU::EBUManager();

queue<Packets::EBUPacketAnalogIn> fromEBU;
queue<EBUPacketAnalogOut> toEBU;

pthread_mutex_t qToEBU = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t qFroEBU = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ebuPAI = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  quit = PTHREAD_COND_INITIALIZER;


//Används för ctrl+c, stänger av allt på ett bättre sätt.
void INT_handler(int dummy){
	timeToQuit = 1;
	exit(EXIT_SUCCESS);
}

/* Needs to get the data out, make some calls to a mapping object
 * and forward everything to the right EBU and the right pin with
 * the right values.
 */


/*
 * Ny lösning, skapa tråd för att ta emot paket till en buffer och en annan tråd för att skicka
 * från buffern. EVERYBODY WINS. behöver mest troligt en buffer för inkommmande data och en
 * för utgående data till/från EBU
 */
void sendToEBU(){
	while(!timeToQuit){
		usleep(100000);
		pthread_mutex_lock(&qToEBU);
		pthread_mutex_lock(&ebuPAI);
		//if queue is empty send full stop, this is more of a failsafe for now.
		if (toEBU.empty()){
			//send stop packet, rely on that the simulator repeats command as long
			//as it wants the command to be sent
		}else{
			packetAnalogIn = toEBU.pop();
		}
		pthread_mutex_unlock(&qToEBU);
		ebuMan.sendCommand(packetAnalogIn);
		pthread_mutex_unlock(&ebuPAI);

	}
}
void receiveFromGateway(Packets::CawlPacket cp,queue<Packets::EBUPacketAnalogIn*> q, EBU::EBUManager man){

}
/*
 * The main function will run threads for receiving packets from the other
 * gateway and another thread that empties the buffer and send packets to the EBU.
 * 2 queues are needed, one from the EBU and one to the EBU
 * One thread to read data from the extendedSocket and put packets INTO the toEBU queue
 * One thread to send data to the EBU from the "toEBU" queue
 * One thread to read data from EBU and put it into the fromEBUqueue
 * One thread to send data to the extendedSocket from the "fromEBU" queue
 */
int main(void)
{
	signal(SIGINT, INT_handler);
	atexit(resetRelays);
	Netapi::CawlSocket gatewaySocket = Netapi::CawlSocket();
	//gemensam quit-variabel för att stänga av snyggt,dvs istället för while(1) så while(!quit)
	if((ebuMan.startConnection(1)) < 0){
		perror("Connection to EBU one has failed, terminating");
	}
	if((ebuMan.startConnection(2)) < 0){
		perror("Connection to EBU one has failed, terminating");
	}

	return 0;
}


