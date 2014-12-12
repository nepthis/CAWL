/*
 * Sim.cpp
 *  Created on: Apr 16, 2014
 *  Author: Robin Bond & H�kan Ther�n
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
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
/*	The method secvSim receives an UDP packet from the simulator and
 * 	inserts the data into a SimPack with a memcpy.
 */
Packets::SimPack Sim::recvSim(void) {
	char recbuf[255];
	Packets::SimPack simpack;
	if(recvfrom(simulatorSocket, recbuf, 255, 0, (struct sockaddr *)&simAddr, &slen)<0){
		errno = EHOSTUNREACH;
		logError(strerror(errno));
		logError("Sim -> recvSim:  Could not receive data from simulator");
		throw errno;
	}
	memcpy(&simpack.fs, recbuf, sizeof(simpack.fs));
	//if(fabs(simpack.getAnalog(LIFTSTICK)) > 0.01){ss.sndPulse();}
	return simpack;
}

/*
 *
 */
void Sim::sendSim(Packets::ImuPack imudata) {
	//Packets going out must have packe ID field of 2001
	imudata.sens.packetId = 2001;
}
/* Sets up the sockets needed to receive data from the simulator and bins the socket.
 * For now it returns false if it fails, true otherwise.
 */
bool Sim::connectToSim() {
	if ((simulatorSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{logWarning("Sim -> connectToSim");return false;}
	memset((char *)&simAddr, 0, slen);
	if(inet_pton(AF_INET, "0.0.0.0", &(simAddr.sin_addr)) < 0)
	{logWarning("Sim -> connectToSim");return false;}
	simAddr.sin_port = htons(65400);
	if (bind(simulatorSocket, (struct sockaddr *)&simAddr, sizeof(simAddr)) < 0)
	{logWarning("Sim -> connectToSim");return false;}
	//---------------------------------------------------------------------------------------------
	if ((motionSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{logWarning("Sim -> connectToSim");return false;}
	memset((char *)&motAddr, 0, slen);
	if(inet_pton(AF_INET, MOV_IP, &(motAddr.sin_addr)) < 0)
	{logWarning("Sim -> connectToSim");return false;}
	motAddr.sin_port = htons(MOV_PORT);
	if (setsockopt(simulatorSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0)
	{logWarning("Sim -> connectToSim");return false;}
	return true;
}
