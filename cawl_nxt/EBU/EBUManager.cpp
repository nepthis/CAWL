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
/*	In the Constructor the connection is set up towards the EBUs, currently just no. 1.
 * 	It's a regular setup for sending UDP Packages, the different ports are in the
 * 	comments inside the constructor.
 */
EBUManager::EBUManager() {
	slen = sizeof(struct sockaddr_in);
	//----------------------------------Sockets------------------------------------------------
	if ((oneAnalogOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw oneAnalogOut; //TBD
	}
	if ((oneRelay = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw oneRelay;

	}
	//------------------------------------------------------------------------------------------------
	//-----------------------------------------ADRESSES-------------------------------------
	//--------------------------------EBUAnalogOut for ebu1------------------------
	memset((char *)&addrOneAnalogOut, 0, sizeof(addrOneAnalogOut));
	inet_pton(AF_INET, "10.0.0.2", &(addrOneAnalogOut.sin_addr));
	addrOneAnalogOut.sin_port = htons(25200);
	//set-up for digital signals will be needed as well in order to control gears

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

/*	Sends an the data-struct inside the analogPacket to the EBU. A switch statements will send the packet either
 * 	to EBU no. 1 or EBU no. 2. The destination EBU is set when creating the packet. Other than that it's a
 * 	regular sendto with UDP
 */
void EBUManager::sendAnalogCommand(Packets::EBUPacketAnalogOut p, int ebuNum){
	Packets::ebuAnOut data = p.getChannel();
	//destinationPort = 25200;
	try{
		switch(ebuNum){
		case 1:
			sendto(oneAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneAnalogOut, slen);
			//	case 2:
			//		sendto(twoAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoAnalogOut, slen);
		}
	}catch(int e){
		printf("Error number: %i\n",e);
		perror("Error sending Analog Packet to the EBU\n");
		throw 1; //TBD
	}
}
/*	The sendRelayCommand works similiar to the sendAnalogCommand function, the difference is that
 * 	the data it sends is a bit different and is used to activate/deactivate relays on the EBU
 */
void EBUManager::sendRelayCommand(Packets::EBURelayPacket rPack, int ebuNum) {
	Packets::EBUrelays data = rPack.getRelays();
	switch(ebuNum){
	case 1:
		sendto(oneRelay, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneRelay, slen);
	}
}

