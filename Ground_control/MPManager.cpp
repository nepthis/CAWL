/*
 * MPManager.cpp
 *
 *  Created on: Dec 9, 2014
 *      Author: cawl
 */
#include "MPManager.h"

namespace Ground_control{
MPManager::MPManager() {
	setupSockets();
}

void MPManager::setupSockets(){
	if ((simsock = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		logError("MPManager: Socket error");
		logError(strerror(errno));
		throw errno;
	}

	memset((char *)&simAddr, 0, sizeof(simAddr));
	inet_pton(AF_INET, SIM_ADDR, &(simAddr.sin_addr));
	simAddr.sin_port = htons(SIM_PORTEN);

}

/*
 * Used to send data to Oryx platform
 */
void MPManager::sendSim(Packets::ImuPack imu) {

	//Sends data to simulator
	if(sendto(simsock, (char*)&imu.sens, 32, 0, (struct sockaddr*) &simAddr, sizeof(struct sockaddr_in)) < 0){
		logError("MPManager: Could not send data");
		logError(strerror(errno));
		throw errno;
	}
}
MPManager::~MPManager() {
}
}
