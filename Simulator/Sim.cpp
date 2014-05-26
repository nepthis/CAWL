/*
 * Sim.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-server
 */

#include "Sim.h"

Sim::Sim() {
	slen = sizeof(simAddr);
	address = ownIP;
	port = 65400;
	realID = 1;

	//Create socket for the simulator
	if ((simulatorSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
	}
	memset((char *)&simAddr, 0, slen);
	inet_pton(AF_INET, ownIP, &(simAddr.sin_addr));
	simAddr.sin_port = htons(65400);

	if (bind(simulatorSocket, (struct sockaddr *)&simAddr, sizeof(simAddr)) < 0) {
		perror("error with bind");
		printf ("Error number is: %s\n",strerror(errno));
	}

}

Sim::~Sim() {
	// TODO Auto-generated destructor stub
}

Packets::SimPack Sim::recPac(void) {
	char recbuf[255];
	Packets::SimPack simpack = Packets::SimPack();
	recvfrom(simulatorSocket, recbuf, 255, 0, (struct sockaddr *)&simAddr, &slen);
	memcpy(&simpack.fromSim, recbuf, sizeof(simpack.fromSim));
	simpack.setID(realID);
	realID++;
	return simpack;
}

void Sim::sendPac() {
}
