/*
 * Sim.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 * Description:
 * 	A simulator object will be responsible for the connection between Ground control
 * 	and the simulator. The object will have functions for sending and receiving data
 * 	between the two systems.
 *
 */

#ifndef SIM_H_
#define SIM_H_
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../Packets/SimPack.h"

#define ownIP "192.168.2.100"	//This is the IP address that the Ground Gateway must have on the network for the simulator
#define simIP "192.168.2.97"	//The Simulators IP address, movement packages does not go here.

namespace Simulator{
/*	The Simulator class Sim is responsible for the communication from (to the motion server later) the simulator.
 * 	Packages from the simulator are received with UDP and will be put into a SimPacket which contains the struct
 * 	for the data received.
 */
class Sim {
	socklen_t slen;
	int simulatorSocket;
	int realID; //The paccketID from the simulator is a bit...weird. Use this instead.
	struct sockaddr_in simAddr;
	struct sockaddr_in ownAddr;
	int simPort;
public:
	Sim();
	virtual ~Sim();
	Packets::SimPack recPac(void);
	void sendPac();
};
}


#endif /* SIM_H_ */
