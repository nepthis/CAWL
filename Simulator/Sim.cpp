/*
 * Sim.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 */

#include "Sim.h"
using namespace Simulator;

Sim::Sim() {
	slen = sizeof(simAddr);
	simPort = 65400;
	realID = 1;
	//Create socket for the simulator
	if ((simulatorSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		throw 13;
	}
	memset((char *)&simAddr, 0, slen);
	inet_pton(AF_INET, "0.0.0.0", &(simAddr.sin_addr));
	simAddr.sin_port = htons(65400);

	if (bind(simulatorSocket, (struct sockaddr *)&simAddr, sizeof(simAddr)) < 0) {
		throw 14;
	}
	printf("Connection to simulator socket was successfull\n");
}


Sim::~Sim() {
	// TODO Auto-generated destructor stub
}

Packets::SimPack Sim::recPac(void) {
	//char recbuf[255];
	Packets::SimPack simpack = Packets::SimPack();
	simpack.fromSim.analog[3] = 1.0;
	simpack.fromSim.analog[2] = 0.5;
	//recvfrom(simulatorSocket, recbuf, 255, 0, (struct sockaddr *)&simAddr, &slen);
	//memcpy(&simpack.fromSim, recbuf, sizeof(simpack.fromSim));
	//printf("data in simPack: %f, %f\n", simpack.fromSim.analog[3],simpack.fromSim.analog[2]  );
	simpack.setID(realID);
	realID++;
	return simpack;
}
/* TODO: Simple sendmsg with UDP to the motion control rig
 *  See report for more information.
 */
void Sim::sendPac() {
}
