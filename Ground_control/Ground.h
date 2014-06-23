/*
 * Ground.h
 *
 *  Created on: May 19, 2014
 *      Author: cawl-server
 */


#ifndef GROUND_H_
#define GROUND_H_

#include <thread>
#include <mutex>
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
#include <errno.h>

#include "../Netapi/CawlSocket.h"
#include "../Packets/EBUPacketAnalogOut.h"
#include "../Simulator/Sim.h"

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


public:
	Netapi::CawlSocket* socketOut;
	Simulator::Sim* simulator;
	Ground(Netapi::CawlSocket* a_socket);

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
