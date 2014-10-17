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

#define connected true

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cmath>

#include "../Packets/SimPack.h"
#include "../Packets/ImuPack.h"
#include "../Ground_control/serSend.h"

#define OWN_IP "192.168.2.100"	//This is the IP address that the Ground Gateway must have on the network for the simulator
#define SIM_IP "192.168.2.97"	//The Simulators IP address, movement packages does not go here.
#define MOV_IP "192.168.2.102"	//Movement packages goes here
#define MOV_PORT 12345
#define SIM_PORT 65400
#define PACKET_ID 2001
#define PACKET_SIZE 32

namespace Simulator{
/*	The Simulator class Sim is responsible for the communication from (to the motion server later) the simulator.
 * 	Packages from the simulator are received with UDP and will be put into a SimPacket which contains the struct
 * 	for the data received.
 */
class Sim {
private:
	socklen_t slen;
	int simulatorSocket;
	int motionSocket;
	int realID; //The packetID from the simulator is a bit...weird. Use this instead.
	struct sockaddr_in simAddr;
	struct sockaddr_in motAddr;
	Packets::ImuPack imuState;
	struct timeval tv;
public:
	Sim();
	virtual ~Sim();
	bool connectToSim();
	Packets::SimPack recvSim(void);
	void sendSim(Packets::ImuPack imudata);
};
}


#endif /* SIM_H_ */
