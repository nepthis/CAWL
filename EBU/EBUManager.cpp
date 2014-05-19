/*
 * EBUManager.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 * Description:
 * 	See the header for more information, this file contains the implementation of the
 * 	functions.
 */

#include "EBUManager.h"

using namespace EBU;

EBUManager::EBUManager() {
	// Constructor
	destinationPort = 0;
	slen = sizeof(struct sockaddr_in);
	//Not going to read data from the EBU. Yet.
	//	if ((oneAnalogIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	//	{
	//		perror("socket error");
	//
	//	}
	//----------------------------------Sockets------------------------------------------------
	if ((oneAnalogOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
	}
	if ((oneRelay = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));

	}
	//------------------------------------------------------------------------------------------------
	//-----------------------------------------ADRESSES-------------------------------------
	//--------------------------------EBUAnalogOut for ebu1------------------------
	memset((char *)&addrOneAnalogOut, 0, sizeof(addrOneAnalogOut));
	inet_pton(AF_INET, "10.0.0.2", &(addrOneAnalogOut.sin_addr)); //Lättare att använda
	addrOneAnalogOut.sin_port = htons(25200);

	//--------------------------------Relays for ebu1--------------------------------------
	memset((char *)&addrOneRelay, 0, sizeof(addrOneRelay));
	inet_pton(AF_INET, "10.0.0.2", &(addrOneRelay.sin_addr)); //Lättare att använda
	addrOneRelay.sin_port = htons(25400); //port for relay data is 25400

	//------------------------------Bind for recv socket ------------------------------------
	/*
	 * only needed for the receiving EBUPacketAnalogIn and digitalin, not done yet.
	if (bind(oneRelay, (struct sockaddr *)&addrOneRelay, sizeof(addrOneRelay)) < 0) {
		perror("error with bind");
		printf ("Error number is: %s\n",strerror(errno));
	}
	 */
	/*
	if (bind(oneAnalogOut, (struct sockaddr *)&addrOneAnalogOut, sizeof(addrOneAnalogOut)) < 0) {
		perror("error with bind");
		printf ("Error number is: %s\n",strerror(errno));
	}
	 */
}

EBUManager::~EBUManager() {
	// TODO Auto-generated destructor stub
}


int EBUManager::sendAnalogCommand(Packets::EBUPacketAnalogOut p, int ebuNum){
	Packets::ebuAnOut data = p.getChannel();
	destinationPort = 25200;
	int error;
	switch(ebuNum){
	case 1:
		sendto(oneAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneAnalogOut, slen);
	}
	return 1; //if success
}

int EBUManager::sendRelayCommand(Packets::EBURelayPacket rPack, int ebuNum) {
	Packets::EBUrelays data = rPack.getRelays();
	//destinationPort = 25200;
	int error;
	switch(ebuNum){
	case 1:
		sendto(oneRelay, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneRelay, slen);
	}
	return 1; //if success
}
/*
int EBUManager::readData(uint16_t pin, uint32_t value){

	return 1; //if success
}
 */
