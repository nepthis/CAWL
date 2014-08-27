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
	//------------------------------------EBU 1--------------------------------------------------
	if ((oneAnalogOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw oneAnalogOut; //TBD
	}
	//------------------------------------EBU 1--------------------------------------------------
	if ((oneDigitalOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw oneAnalogOut; //TBD
	}
	//------------------------------------EBU 1--------------------------------------------------
	if ((oneRelay = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw oneRelay;
	}
	//------------------------------------EBU 2--------------------------------------------------
	if ((twoAnalogOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw twoAnalogOut; //TBD
	}
	//------------------------------------EBU 2--------------------------------------------------
	if ((twoDigitalOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw twoDigitalOut; //TBD
	}
	//------------------------------------EBU 2--------------------------------------------------
	if ((twoRelay = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw twoRelay;
	}

	//------------------------------------------------------------------------------------------------
	//-----------------------------------------ADRESSES-------------------------------------
	//-------------------------------EBUAnalogOut for EBU 1-----------------------
	memset((char *)&addrOneAnalogOut, 0, sizeof(addrOneAnalogOut));
	inet_pton(AF_INET, "10.0.0.2", &(addrOneAnalogOut.sin_addr));
	addrOneAnalogOut.sin_port = htons(25200);
	//--------------------------------EBUDigitalOut for EBU 1-----------------------
	memset((char *)&addrOneDigitalOut, 0, sizeof(addrOneDigitalOut));
	inet_pton(AF_INET, "10.0.0.2", &(addrOneDigitalOut.sin_addr));
	addrOneDigitalOut.sin_port = htons(25300);
	//-------------------------------Relays for EBU 1-------------------------------------
	memset((char *)&addrOneRelay, 0, sizeof(addrOneRelay));
	inet_pton(AF_INET, "10.0.0.2", &(addrOneRelay.sin_addr));
	addrOneRelay.sin_port = htons(25400);

	//--------------------------------EBUAnalogOut for EBU 2------------------------
	memset((char *)&addrTwoAnalogOut, 0, sizeof(addrTwoAnalogOut));
	inet_pton(AF_INET, "10.0.0.3", &(addrTwoAnalogOut.sin_addr));
	addrTwoAnalogOut.sin_port = htons(25200);
	//--------------------------------EBUDigitalOut for EBU 2-----------------------
	memset((char *)&addrTwoDigitalOut, 0, sizeof(addrTwoDigitalOut));
	inet_pton(AF_INET, "10.0.0.3", &(addrTwoDigitalOut.sin_addr));
	addrTwoDigitalOut.sin_port = htons(25300);
	//-------------------------------Relays for EBU 2--------------------------------------
	memset((char *)&addrTwoRelay, 0, sizeof(addrTwoRelay));
	inet_pton(AF_INET, "10.0.0.3", &(addrTwoRelay.sin_addr));
	addrTwoRelay.sin_port = htons(25400);


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
}
/*	Sends an the data-struct inside the analogPacket to the EBU. A switch statements will send the packet either
 * 	to EBU no. 1 or EBU no. 2. The destination EBU is set when creating the packet. Other than that it's a
 * 	regular sendto with UDP
 */
void EBUManager::sendAnalogCommand(Packets::ebuAnOut data, int ebuNum){
	try{
		switch(ebuNum){
		case 1:
			sendto(oneAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneAnalogOut, slen);
		case 2:
			sendto(twoAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoAnalogOut, slen);
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
	try{
		switch(ebuNum){
		case 1:
			sendto(oneRelay, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneRelay, slen);
		case 2:
			sendto(twoRelay, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoRelay, slen);
		}
	}catch(int e){
		printf("Error number: %i\n",e);
		perror("Error sending relay packet to an EBU\n");
		throw 1; //TBD
	}

}

void EBU::EBUManager::sendDigitalCommand(Packets::EBUdigitalOut data, int ebuNum) {
	try{
		switch(ebuNum){
		case 1:
			sendto(oneDigitalOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneDigitalOut, slen);
		case 2:
			sendto(twoDigitalOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoDigitalOut, slen);
		}
	}catch(int e){
		printf("Error number: %i\n",e);
		perror("Error sending Analog Packet to the EBU\n");
		throw 1; //TBD
	}
}
