/*
 * Ground_control.h
 *
 *  Created on: May 16, 2014
 *      Author: cawl-server
 */

#ifndef GROUND_CONTROL_H_
#define GROUND_CONTROL_H_
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
typedef struct dataCont{
	uint8_t type;
	uint8_t pin;
	uint8_t value;
	uint8_t ebuNum;
}dataCont;


//These should preferrably be in a struct and passed to the functions in question...
int countBoomUp = 0;
int countBuckUp = 0;
int countBoomDown = 0;
int countBuckDown = 0;
dataCont dc = {0,0,0,0};
char g_buffer[256]= {0};
int pleased = 0;





#endif /* GROUND_CONTROL_H_ */
