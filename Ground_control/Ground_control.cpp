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

#include <pthread.h>
#include <queue>
#include "../Packets/CawlPacket.h"
using namespace std;

//Globals, because threads. Could and should make a struct for these if there is time left.
Packets::CawlPacket cPack = Packets::CawlPacket();
Packets::CawlPacket pktOut = Packets::CawlPacket();

Netapi::CawlSocket socketOut = Netapi::CawlSocket();

queue<Packets::CawlPacket> toGateWay;

pthread_mutex_t qCawl = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t packet = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t packetOut = PTHREAD_MUTEX_INITIALIZER;

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
char g_buffer[32];

int pleased = 0;



/*
 * enQueuePacket takes the data and a char buffer and copies in the data into the buffer
 * afterwards it creates a packet and copies it into a queue. (I think it copies the packet, at least)
 */
void enQueuePacket(char t, uint8_t p, uint8_t v,uint8_t n, char buffer[32]){
	memcpy(buffer+0, &t, 1);
	memcpy(buffer+1, &p, 1);
	memcpy(buffer+2, &v, 1);
	memcpy(buffer+3, &n, 1);
	pthread_mutex_lock(&packet);
	//set values in packet
	pthread_mutex_lock(&qCawl);
	toGateWay.push(cPack);
	pthread_mutex_unlock(&qCawl);
	pthread_mutex_unlock(&packet);
}
void boomUp(uint8_t value, char buffer[]){
	char type ="A";
	uint8_t pin = 9;
	uint8_t ebuNum = 1;
	enQueuePacket(type, pin, value, ebuNum,buffer);

}
void boomDown(uint8_t value, char buffer[]){
	char type ="A";
	uint8_t pin = 10;
	uint8_t ebuNum = 1;
	enQueuePacket(type, pin, value, ebuNum, buffer);
}
void bucketUp(uint8_t value, char buffer[]){
	char type ="A";
	uint8_t pin = 11; //need to look this up
	uint8_t ebuNum = 1;
	enQueuePacket(type, pin, value, ebuNum, buffer);
}
void bucketDown(uint8_t value, char buffer[]){
	char type ="A";
	uint8_t pin = 12;
	uint8_t ebuNum = 1;
	enQueuePacket(type, pin, value, ebuNum, buffer);
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
	while(((c = getch()) != EOF) or (not pleased)){
		c = tolower(c);
		switch(c){
		case 'w':
			putchar(c);
			countBoomDown = 0;
			value = countBoomDown;
			boomDown(value, g_buffer);
			if (countBoomUp < 5)
			{
				countBoomUp++;
				printf("\nBoom level: %d\n", countBoomUp);
				value = countBoomUp;
				boomUp(value,g_buffer);
			}
			else
			{
				printf("\nBoom at max level\n");
			}
			break;

		case 's':
			putchar(c);
			countBoomUp = 0;
			value = countBoomUp;
			boomUp(value, g_buffer);
			if(countBoomDown > -5)
			{
				countBoomDown--;
				printf("\nBoom level: %d\n", countBoomDown);
				value = countBoomDown*-1;
				boomDown(value,g_buffer);
			}
			else
			{
				printf("\nBoom at min level\n");
			}
			break;
		case 'q':
			putchar(c);
			countBuckDown = 0;
			value = countBuckDown;
			bucketDown(value, g_buffer);
			if(countBuckUp < 5)
			{
				countBuckUp++;
				printf("\nBucket level: %d\n", countBuckUp);
				value = countBuckUp;
				bucketUp(value, g_buffer);
			}
			else
			{
				printf("\nBucket at max level\n");
			}
			break;

		case 'e':
			putchar(c);
			countBuckUp = 0;
			if(countBuckDown > -5)
			{
				countBuckDown--;
				printf("\nBucket level: %d\n", countBuckDown);
				value = countBuckDown*-1;
				bucketDown(value,g_buffer);
			}
			else
			{
				printf("\nBucket at min level\n");
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
	pthread_exit(NULL);
}
/*
 * Function for sending the data through the CAWL socket to the other Gateway
 * Locks the FIFO queue and the packet required to hold the data temporarily
 * and then sends it.
 */
void *deQueue(void *parg){
	while(not pleased){
		pthread_mutex_lock(&qCawl);
		pthread_mutex_lock(&packetOut);
		pktOut = toGateWay.pop();
		pthread_mutex_unlock(&qCawl);
		socketOut.send(&pktOut);
		pthread_mutex_unlock(&packetOut);
	}
	pthread_exit(NULL);
}

int main()
{
	printf("w = raise boom\ts = lower boom\nq = tilt bucket up\te = tilt bucket down\n");
	printf("z = reset boom level\tx = reset bucket level\n");
	pthread_t inputHandler;
	pthread_t dataSender;
	pthread_create(&inputHandler, NULL, handleInput, NULL);
	pthread_create(&dataSender, NULL, deQueue, NULL);

	printf("Finished");
	return 0;
}

