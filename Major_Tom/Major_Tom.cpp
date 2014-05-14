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
#define R_S19 	0
#define R_S20 	1
#define R_P15 	2
#define R_P16 	3

#define R_S21 	4
#define R_res1	5
#define R_res2	6
#define R_res3	7

#define R_S15	8
#define R_S16 	9
#define R_P11 	10
#define R_P12 	11

#define R_S17 	12
#define R_S18 	13
#define R_P13 	14
#define R_P14 	15

#define R_S11 	16
#define R_S12 	17
#define R_P7 	18
#define R_P8 	19

#define R_S13 	20
#define R_S14 	21
#define R_P9 	22
#define R_P10 	23

#define R_S7 	24
#define R_S8 	25
#define R_P3 	26
#define R_P4 	27

#define R_S9 	28
#define R_S10 	29
#define R_P5 	30
#define R_P6 	31

#define R_S1 	32
#define R_S2 	33
#define R_S3 	34
#define R_S4 	35

#define R_S5 	36
#define R_S6 	37
#define R_P1 	38
#define R_P2 	39

#define R_F2 	40
#define R_D37 	41
#define R_F3 	42
#define R_D38 	43

#define R_F4 	44
#define R_D39 	45
#define R_F5 	46
#define R_F6 	47

#define R_D21 	48
#define R_CAN1 	49
#define R_J1708 50
#define R_CAN2 	51

#define R_D34 	52
#define R_D35 	53
#define R_F1 	54
#define R_D36 	55

#define R_D30 	56
#define R_D17 	57
#define R_D31 	58
#define R_D18 	59

#define R_D32 	60
#define R_D19 	61
#define R_D33 	62
#define R_D20 	63

#define R_D7 	64
#define R_D27 	65
#define R_D14 	66
#define R_D8 	67

#define R_D28 	68
#define R_D15 	69
#define R_D29 	70
#define R_D16 	71

#define R_D4 	72
#define R_D11 	73
#define R_D5 	74
#define R_D25 	75

#define R_D12 	76
#define R_D6 	77
#define R_D26 	78
#define R_D13 	79

#define R_D1 	80
#define R_D22 	81
#define R_D2 	82
#define R_D9 	83

#define R_D23 	84
#define R_D3 	85
#define R_D10 	86
#define R_D24 	87

#define R_A11 	88
#define R_A19 	89
#define R_A6 	90
#define R_A12 	91

#define R_A20 	92
#define R_A7 	93
#define R_A13 	94
#define R_res4 	95

#define R_A3 	96
#define R_A9 	97
#define R_A17 	98
#define R_A4 	99

#define R_A10 	100
#define R_A18 	101
#define R_A5 	102
#define R_res5 	103

#define R_A14 	104
#define R_A1 	105
#define R_A15 	106
#define R_A2 	107

#define R_A8 	108
#define R_A16 	109
#define R_res6 	110
#define R_res7 	111

using namespace std;
//Globals

int timeToQuit = 0;

Packets::EBUPacketAnalogOut packetAnalogOut = Packets::EBUPacketAnalogOut();
Packets::EBUPacketAnalogOut stopPacket = Packets::EBUPacketAnalogOut();
Packets::CawlPacket cPack = Packets::CawlPacket();;
Packets::EBURelayPacket rPack = Packets::EBURelayPacket();

EBU::EBUManager ebuMan = EBU::EBUManager();

queue<Packets::EBUPacketAnalogIn> fromEBU;
queue<Packets::EBUPacketAnalogOut> AnalogToEBU;

pthread_mutex_t qToEBU = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t qFroEBU = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t ebuPAO = PTHREAD_MUTEX_INITIALIZER;

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
 * för utgående data till/från EBU ??? Profit
 */
void sendToEBU(){
	while(!timeToQuit){
		//The sleep is to make sure that we don't flood the EBU, and since the EBU responds with 10Hz then will do the same.
		usleep(100000);
		pthread_mutex_lock(&qToEBU);
		pthread_mutex_lock(&ebuPAO);
		//if queue is empty send full stop, this is more of a failsafe for now.
		if (AnalogToEBU.empty()){
			packetAnalogOut = stopPacket;
		}else{
			packetAnalogOut = AnalogToEBU.pop();
		}
		pthread_mutex_unlock(&qToEBU);
		ebuMan.sendAnalogCommand(packetAnalogOut, 1);
		pthread_mutex_unlock(&ebuPAO);

	}
}
void receiveFromGateway(){
//Packets::CawlPacket cp,queue<Packets::EBUPacketAnalogIn*> q, EBU::EBUManager man
}


void resetRelays(){
	rPack = Packets::EBURelayPacket(); //nollvärden för allt
	ebuMan.sendRelayCommand(rPack, 1);
	//ebuMan.sendRelayCommand(rPack, 2);
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

/*
	pthread_t ebuSender;
	pthread_t gateWayReceiver;
 */

	int toggle = 0;
	while(1){
		sleep(1);
		if(toggle){

		}


	}
	return 0;
}


