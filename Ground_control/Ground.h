/*
 * Ground.h
 *
 *  Created on: May 19, 2014
 *      Author: cawl-server
 */


#ifndef GROUND_H_
#define GROUND_H_

#include <pthread.h>
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

#include "../Netapi/CawlSocket.h"
#include "../Packets/EBUPacketAnalogOut.h"
#include "../Simulator/Sim.h"



#define address1 "127.0.0.1"
#define address2 "121.0.0.1"


class Ground {
private:
	int countBoomUp, countBuckUp, countBoomDown, countBuckDown;
	int pleased;
	//Netapi::Host client;

public:
	Netapi::CawlSocket* socketOut;
	Simulator::Sim* simulator;
	Ground(Netapi::CawlSocket* a_socket);
	//void handleInput(void);
	//Packets::CawlPacket createPacket(uint8_t type,uint8_t  pin, uint8_t value,uint8_t ebuNumber);
	int PacketHandler();
	int setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao);
	int sendPacket(int prio, int streamID,  Packets::EBUPacketAnalogOut pkt);
	int setBoom(float value, Packets::EBUPacketAnalogOut* pkt);
	int setBucket(float value, Packets::EBUPacketAnalogOut* pkt);
//	int boomUp(uint8_t voltage);
//	int boomDown(uint8_t voltage);
//	int bucketUp(uint8_t voltage);
//	int bucketDown(uint8_t voltage);
	char getch();
	virtual ~Ground();
};

#endif /* GROUND_H_ */
