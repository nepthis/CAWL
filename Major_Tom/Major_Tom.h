/*
 * Major_Tom.h
 *
 *  Created on: May 16, 2014
 *      Author: Robin Bond
 *	Description: This header file contains all globals and variables used in
 *	the .cpp file + all includes and defines
 */
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

#include "../Netapi/CawlSocket.h"
#include"../EBU/EBUManager.h"
#include "../Packets/CawlPacket.h"
//#include "../Packets/EBUPacketAnalogIn.h"
#include "../Packets/EBUPacketAnalogOut.h"

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




#ifndef MAJOR_TOM_H_
#define MAJOR_TOM_H_
typedef struct dataCont{
	uint8_t type;
	uint8_t pin;
	uint8_t value;
	uint8_t ebuNum;
}dataCont;

//Globals

dataCont dc = {0,0,0,0};
int timeToQuit = 0;

Packets::EBUPacketAnalogOut packetAnalogOut = Packets::EBUPacketAnalogOut();
Packets::EBUPacketAnalogOut stopPacket = Packets::EBUPacketAnalogOut();
Packets::CawlPacket cPack = Packets::CawlPacket(0);
Packets::CawlPacket tempPack = Packets::CawlPacket(0);
Packets::EBURelayPacket rPack = Packets::EBURelayPacket();


EBU::EBUManager ebuMan = EBU::EBUManager();
Netapi::Host h = Netapi::Host((char*)"130.240.109.70", 1235, (char*)"130.240.109.70", true);
Netapi::CawlSocket gatewaySocket = Netapi::CawlSocket(h);

queue<Packets::CawlPacket> packetBuffer;;

pthread_mutex_t m_packetBuffer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_PacketAnalogOut = PTHREAD_MUTEX_INITIALIZER;
//Mutex for the cawlsocket will be needed later for twoway communication
//pthread_mutex_t m_gwSocket = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_ebuMan = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t m_cawlPacket = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t  quit = PTHREAD_COND_INITIALIZER;




#endif /* MAJOR_TOM_H_ */
