/*
 * Sim.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 */
#include "Sim.h"
#include "../logger.h"
using namespace Ground_control;
Sim::Sim() {
	slen = sizeof(simAddr);
	realID = 1;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	//ss = serSend();
}
Sim::~Sim() {
}
/*	The method recPac reveices a UDP packet from the simulator and
 * 	inserts the data into a SimPack with a memcpy. The SimPack will
 * 	get a new ID because the simulator returns the same one.
 */
Packets::SimPack Sim::recvSim(void) {
	char recbuf[255];
	Packets::SimPack simpack;
	if(recvfrom(simulatorSocket, recbuf, 255, 0, (struct sockaddr *)&simAddr, &slen)<0){
		logError("Sim -> recvSim");
		logError(strerror(errno));
		throw errno;
	}
	memcpy(&simpack.fs, recbuf, sizeof(simpack.fs));
	simpack.stampTime();
	simpack.setID(realID);
	realID++;
	//if(fabs(simpack.getAnalog(LIFTSTICK)) > 0.01){ss.sndPulse();}
	return simpack;
}
void Sim::sendSim(Packets::ImuPack imudata) {
	//Packets going out must have packe ID field of 2001
	imudata.sens.packetId = 2001;
}

bool Sim::connectToSim() {
	//Create socket for the simulator
	if ((simulatorSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{logWarning("Sim -> connectToSim");logWarning(strerror(errno));return false;}
	memset((char *)&simAddr, 0, slen);
	if(inet_pton(AF_INET, "0.0.0.0", &(simAddr.sin_addr)) < 0)
	{logWarning("Sim -> connectToSim");logWarning(strerror(errno));return false;}
	simAddr.sin_port = htons(65400);
	if (bind(simulatorSocket, (struct sockaddr *)&simAddr, sizeof(simAddr)) < 0)
	{logWarning("Sim -> connectToSim");logWarning(strerror(errno));return false;}
	//---------------------------------------------------------------------------------------------
	if ((motionSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{logWarning("Sim -> connectToSim");logWarning(strerror(errno));return false;}
	memset((char *)&motAddr, 0, slen);
	if(inet_pton(AF_INET, MOV_IP, &(motAddr.sin_addr)) < 0)
	{logWarning("Sim -> connectToSim");logWarning(strerror(errno));return false;}
	motAddr.sin_port = htons(MOV_PORT);
	if (setsockopt(simulatorSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
	{logWarning("Sim -> connectToSim");logWarning(strerror(errno));return false;}
	return true;
}
