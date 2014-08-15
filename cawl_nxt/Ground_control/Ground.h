/*
 * Ground.h
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */


#ifndef GROUND_H_
#define GROUND_H_

#include <thread>	//for std::thread
#include <mutex>	//std mutex and unique locks
#include <chrono>

#include <stdio.h>	//for printf
#include <string.h>	//std::string
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <errno.h>	//For raising errors
#include <cstring>
//Own classes are included here
#include "../Netapi/CawlSocket.h"
#include "../Packets/EBUPacketAnalogOut.h"
#include "../Simulator/Sim.h"


/*	The class ground is responsible for maintaining a connection with the simulator (sim.h)
 * 	and to translate packages from the simulator into packets for the EBU and then to send
 * 	them to the Mobile gateway using a CawlSocket.
 * 	The methods startReceive and startSend are started are made for being started in threads
 * 	and will keep on receiving packages from the simulator and sending them over the cawlsocket.
 * 	TODO: Use a state + a compare function to see if sending a new packet is necessary.
 */
class Ground {
private:
	int countBoomUp, countBuckUp, countBoomDown, countBuckDown;
	std::mutex m_cawlSocket;
	Packets::EBUPacketAnalogOut epao;
	Packets::SimPack sp;
	Packets::CawlPacket *out;
	Packets::CawlPacket *in;
	char *thetemp;
	char *state;
	Netapi::Host h;
	Netapi::CawlSocket *socketOut;
	float tempValue;


public:
	Simulator::Sim* simulator;
	Ground(char* addressOne, char* addressTwo);
	void startRecieve();
	void startSend();
	void receivePacket();
	void setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao);
	void sendPacket();
	int setBoom(float value, Packets::EBUPacketAnalogOut* pkt);
	int setBucket(float value, Packets::EBUPacketAnalogOut* pkt);
	virtual ~Ground();
};

#endif /* GROUND_H_ */
