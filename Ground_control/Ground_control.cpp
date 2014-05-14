/*
 * Ground_control.cpp
 * Author: Robin Bond
 * Description:
 * 	Main program for creating objects for simulator and communication for the
 * 	gateway at the control site.
 *
 */

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <signal.h>

#include <pthread.h>
#include <queue>

#include "../Packets/CawlPacket.h"
//#include "../Packets/CawlPacket.cpp"
#include "../Packets/EBUPacketAnalogOut.h"
#include "../Netapi/CawlSocket.h"
#include "../Netapi/Host.h"

using namespace std;

//Globals, because threads. Could and should make a struct for these if there is time left.
//Packets::CawlPacket cPack = Packets::CawlPacket();

char * tempbuff = {0};

Packets::CawlPacket pktOut = Packets::CawlPacket((uint8_t)1,(uint8_t)1,tempbuff);
//Packets::EBUPacketAnalogOut  analogPacket = Packets::EBUPacketAnalogOut();
Netapi::Host client = Netapi::Host((char*)"127.0.0.1", 1235, (char*)"127.0.0.1");


Netapi::CawlSocket socketOut = Netapi::CawlSocket(client);

//EBU::EBUManager em = EBU::EBUManager();

queue<Packets::CawlPacket> toGateWay;

pthread_mutex_t qCawl = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t packet = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t packetOut = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t m_ebuman = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  quit = PTHREAD_COND_INITIALIZER;

//These should preferrably be in a struct and passed to the functions in question...
int c;
int countBoomUp = 0;
int countBuckUp = 0;
int countBoomDown = 0;
int countBuckDown = 0;
char type;
uint8_t pin;
uint8_t value;
uint8_t ebuNum;
char g_buffer[64]= {0};
int pleased = 0;



/*
 * enQueuePacket takes the data and a char buffer and copies in the data into the buffer
 * afterwards it creates a packet and copies it into a queue. (I think it copies the packet, at least)
 */
void *deQueue(void *parg){
	while(not pleased){
		pthread_mutex_lock(&qCawl);
		pthread_mutex_lock(&packetOut);
		pktOut = toGateWay.front();
		toGateWay.pop();
		pthread_mutex_unlock(&qCawl);
		socketOut.send(pktOut);
		//em.sendAnalogCommand()
		pthread_mutex_unlock(&packetOut);
	}
	pthread_cond_signal(&quit);
	pthread_exit(NULL);
}
void enQueuePacket(char t, int  p, int v,int n, char buffer[64]){
	memcpy(buffer+0, &t, sizeof(char));
	memcpy(buffer+8, &p, sizeof(int));
	memcpy(buffer+24, &v, sizeof(int));
	memcpy(buffer+40, &n, sizeof(int));
	pthread_mutex_lock(&packetOut);
	pktOut.data=buffer;
	pktOut.prio = 1;
	pktOut.streamId = 1;
	pthread_mutex_lock(&qCawl);
	toGateWay.push(pktOut);
	pthread_mutex_unlock(&qCawl);
	pthread_mutex_unlock(&packetOut);
}
void boomUp(int voltage, char buffer[64]){
	char type ='a';
	int pin = AO_9;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum, buffer);
	//enQueuePacket(type, pin, value, ebuNum,buffer);
	//pthread_mutex_lock(&packet);
	//pthread_mutex_lock(&m_ebuman);
	//printf("\nvoltage to be written: %d\n", voltage);
	//analogPacket.setChannelValue(voltage, AO_9);
	//sleep(0);
	//uint16_t temp = analogPacket.getChannelValue(AO_9);
	//printf("\nthe value in the array is: %u\n", temp);
	//em.sendAnalogCommand(analogPacket,ebuNum);
	//pthread_mutex_unlock(&m_ebuman);
	//pthread_mutex_unlock(&packet);


}
void boomDown(int voltage, char buffer[64]){
	char type ='a';
	int pin = AO_10;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum, buffer);
	//enQueuePacket(type, pin, value, ebuNum, buffer);
	//	pthread_mutex_lock(&packet);
	//	pthread_mutex_lock(&m_ebuman);
	//	printf("\nvoltage to be written: %d\n", voltage);
	//	analogPacket.setChannelValue(voltage, AO_10);
	//	sleep(1);
	//	uint16_t temp = analogPacket.getChannelValue(AO_10);
	//	printf("\nthe value in the array is: %u\n", temp);
	//	em.sendAnalogCommand(analogPacket,ebuNum);
	//	pthread_mutex_unlock(&m_ebuman);
	//	pthread_mutex_unlock(&packet);
}
void bucketUp(int voltage, char buffer[64]){
	char type ='a';
	int pin = AO_11;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum, buffer);
	//enQueuePacket(type, pin, value, ebuNum, buffer);
	//	pthread_mutex_lock(&packet);
	//	pthread_mutex_lock(&m_ebuman);
	//	analogPacket.setChannelValue(voltage, AO_11);
	//	em.sendAnalogCommand(analogPacket,ebuNum);
	//	pthread_mutex_unlock(&m_ebuman);
	//	pthread_mutex_unlock(&packet);
}
void bucketDown(int voltage, char buffer[64]){
	char type ='a';
	int pin = AO_12;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum, buffer);
	//enQueuePacket(type, pin, value, ebuNum, buffer);
	//	pthread_mutex_lock(&packet);
	//	pthread_mutex_lock(&m_ebuman);
	//	analogPacket.setChannelValue(voltage, AO_12);
	//	em.sendAnalogCommand(analogPacket,ebuNum);
	//	pthread_mutex_unlock(&m_ebuman);
	//	pthread_mutex_unlock(&packet);
}

/*
 * Borrowed code for making key presses register without hitting enter.
 * Borrowed from http://stackoverflow.com/a/912796, is used instead of
 * This function is used instead of getchar() from stdio.h.
 */
char getch() {
	char buf = 0;
	struct termios old = {0};
	if (tcgetattr(0, &old) < 0)
		perror("tcsetattr()");
	old.c_lflag &= ~ICANON;
	old.c_lflag &= ~ECHO;
	old.c_cc[VMIN] = 1;
	old.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &old) < 0)
		perror("tcsetattr ICANON");
	if (read(0, &buf, 1) < 0)
		perror ("read()");
	old.c_lflag |= ICANON;
	old.c_lflag |= ECHO;
	if (tcsetattr(0, TCSADRAIN, &old) < 0)
		perror ("tcsetattr ~ICANON");
	return (buf);
}
/*
pressing the button for the desired function will increment voltage with 1.
Max level is 5 and pushing "opposite" will result decrease with 1.
IMPORTANT! This is only for keyboard control, will be different when in use
with a joystick or similar from the simulator.
 */
void *handleInput(void *parg){
	while((c = getch()) != EOF){
		c = tolower(c);
		switch(c){
		case 'w':
			putchar(c);
			countBoomDown = 0;
			boomDown(countBoomDown, g_buffer);
			if (countBoomUp < 5)
			{
				countBoomUp++;
				printf("\nBoom level: %d\n", countBoomUp);
				boomUp(countBoomUp,g_buffer);
			}
			else
			{
				printf("\nBoom at max level\n");
				boomUp(countBoomUp,g_buffer);
			}
			break;

		case 's':
			putchar(c);
			countBoomUp = 0;
			boomUp(0, g_buffer);
			if(countBoomDown > -5)
			{
				countBoomDown--;
				printf("\nBoom level: %d\n", countBoomDown);
				boomDown((countBoomDown*-1),g_buffer);
			}
			else
			{
				printf("\nBoom at min level\n");
				boomDown((countBoomDown*-1),g_buffer);
			}
			break;
		case 'q':
			putchar(c);
			countBuckDown = 0;
			bucketDown(countBuckDown, g_buffer);
			if(countBuckUp < 5)
			{
				countBuckUp++;
				printf("\nBucket level: %d\n", countBuckUp);
				bucketUp(countBuckUp, g_buffer);
			}
			else
			{
				printf("\nBucket at max level\n");
				bucketUp(countBuckUp, g_buffer);
			}
			break;

		case 'e':
			putchar(c);
			countBuckUp = 0;
			bucketUp(countBuckUp, g_buffer);
			if(countBuckDown > -5)
			{
				countBuckDown--;
				printf("\nBucket level: %d\n", countBuckDown);
				bucketDown(countBuckDown,g_buffer);
			}
			else
			{
				printf("\nBucket at min level\n");
				bucketDown((countBuckDown*-1),g_buffer);
			}
			break;
			//		case 'z':
			//			putchar(c);
			//			countBoomDown, countBoomUp = 0;
			//			printf("\nBoom level reset\n");
			//			break;
			//		case 'x':
			//			putchar(c);
			//			countBuckDown, countBuckUp = 0;
			//
			//			printf("\nBucket level reset\n");
			//			break;

		default:
			printf("\nUnrecognized input\n");
			break;
		}
	}
	printf("main loop failed");
	pthread_cond_signal(&quit);
	pthread_exit(NULL);
}
/*
 * Function for sending the data through the CAWL socket to the other Gateway
 * Locks the FIFO queue and the packet required to hold the data temporarily
 * and then sends it.
 */

void INT_handler(int dummy){
	pleased = 1;
	exit(EXIT_SUCCESS);
}
int main()
{

	signal(SIGINT, INT_handler);
	printf("w = raise boom\ts = lower boom\nq = tilt bucket up\te = tilt bucket down\n");
	//	Packets::EBURelayPacket rp = Packets::EBURelayPacket();
	//	rp.setRelayValue(R_A9, 0);
	//	rp.setRelayValue(R_A10, 0);
	//	rp.setRelayValue(R_A11, 0);
	//	rp.setRelayValue(R_A12, 0);
	//	printf("values of the array are %d, %d, %d, %d\n",rp.getRelayValue(R_A9),rp.getRelayValue(R_A10),rp.getRelayValue(R_A11),rp.getRelayValue(R_A12));
	//	rp.setRelayValue(R_A9, 1);
	//	rp.setRelayValue(R_A10, 1);
	//	rp.setRelayValue(R_A11, 1);
	//	rp.setRelayValue(R_A12, 1);
	//	printf("The values set are %d, %d, %d and %d\n", R_A9, R_A10, R_A11, R_A12);
	//	printf("values of the array are %d, %d, %d, %d\n",rp.getRelayValue(R_A9),rp.getRelayValue(R_A10),rp.getRelayValue(R_A11),rp.getRelayValue(R_A12));
	//
	//	sleep(1);
	//	printf("relay packet is configured, will send in 2s\n");
	//	sleep(2);
	//	em.sendRelayCommand(rp,1);
	//	printf("Packet sent. Creating worker-thread\n");

	pthread_t t1;
	pthread_t t2;
	int err1 = pthread_create(&t1, NULL, handleInput, NULL);
	int err2 = pthread_create(&t2, NULL, deQueue, NULL);

	if (err1 or err2){
		perror("Something went wrong with the thread");
	}
	printf("Threads created, awaiting input");
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);

	printf("Finished");
	return 0;
}

