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
using namespace Packets;
using namespace std;
/*	In the Constructor the connection is set up towards the EBUs, currently just no. 1.
 * 	It's a regular setup for sending UDP Packages, the different ports are in the
 * 	comments inside the constructor.
 */
EBUManager::EBUManager() {
	slen = sizeof(struct sockaddr_in);
	tv.tv_sec = 1;
	tv.tv_usec = 0;
}
EBUManager::~EBUManager() {
}
/*	Sends an the data-struct inside the analogPacket to the EBU. A switch statements will send the packet either
 * 	to EBU no. 1 or EBU no. 2. The destination EBU is set when creating the packet. Other than that it's a
 * 	regular sendto with UDP
 */
void EBUManager::sendAnalogCommand(ebuAnOut data, int ebuNum){
	switch(ebuNum){
	case 1:
		sendto(oneAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneAnalogOut, slen);
		break;
	case 2:
		sendto(twoAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoAnalogOut, slen);
		break;
	}
}
/*	The sendRelayCommand works similiar to the sendAnalogCommand function, the difference is that
 * 	the data it sends is a bit different and is used to activate/deactivate relays on the EBU
 * 	by receiving data from the EBU we prevent it from crashing... according to it's creator.
 */
void EBUManager::sendRelayCommand(RelayOut rPack, int ebuNum) {
	EBUrelays data = rPack.getRelays();
	char * buffer[14];
	memcpy(buffer, &data, 14);
	switch(ebuNum){
	case 1:
		sendto(oneRelay,buffer, 14, 0, (struct sockaddr*) &addrOneRelay, slen);
		break;
	case 2:
		sendto(twoRelay, buffer, 14, 0, (struct sockaddr*) &addrTwoRelay, slen);
		break;
	}
}

void EBU::EBUManager::sendDigitalCommand(EBUdigitalOut data, int ebuNum) {
	switch(ebuNum){
	case 1:
		sendto(oneDigitalOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneDigitalOut, slen);
		break;
	case 2:
		sendto(twoDigitalOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoDigitalOut, slen);
		break;
	}

}
//Read this at your own risk..
//UDP connection, must write SCTP vers
bool EBU::EBUManager::connectToEBU() {
	try{
		//----------------------------------Sockets------------------------------------------------
		//------------------------------------EBU 1--------------------------------------------------
		//--------------------------------AnalogOut----------------------------------------------
		if ((oneAnalogOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//--------------------------------DigitalOut----------------------------------------------
		if ((oneDigitalOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//----------------------------------Relays--------------------------------------------------
		if ((oneRelay = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//---------------------------------AnalogIn-----------------------------------------------
		if ((sockOneAnalogIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//---------------------------------DigitalIn-----------------------------------------------
		if ((sockOneDigitalIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//------------------------------------EBU 2--------------------------------------------------
		//--------------------------------AnalogOut----------------------------------------------
		if ((twoAnalogOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//--------------------------------DigitalOut----------------------------------------------
		if ((twoDigitalOut = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//----------------------------------Relays--------------------------------------------------
		if ((twoRelay = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//---------------------------------AnalogIn-----------------------------------------------
		if ((sockTwoAnalogIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//---------------------------------DigitalIn-----------------------------------------------
		if ((sockTwoDigitalIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
		{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
		//------------------------------------------------------------------------------------------------
		//-----------------------------------------ADRESSES-------------------------------------
		//-------------------------------EBUAnalogOut for EBU 1-----------------------
		memset((char *)&addrOneAnalogOut, 0, sizeof(addrOneAnalogOut));
		inet_pton(AF_INET, EBU_IP_1, &(addrOneAnalogOut.sin_addr));
		addrOneAnalogOut.sin_port = htons(PORT_ANALOG_OUT);
		//--------------------------------EBUDigitalOut for EBU 1-----------------------
		memset((char *)&addrOneDigitalOut, 0, sizeof(addrOneDigitalOut));
		inet_pton(AF_INET, EBU_IP_1, &(addrOneDigitalOut.sin_addr));
		addrOneDigitalOut.sin_port = htons(PORT_DIGITAL_OUT);
		//-------------------------------Relays for EBU 1-------------------------------------
		memset((char *)&addrOneRelay, 0, sizeof(addrOneRelay));
		inet_pton(AF_INET, EBU_IP_1, &(addrOneRelay.sin_addr));
		addrOneRelay.sin_port = htons(PORT_RELAYS);
		//-------------------------------EBUAnalogIn for EBU 1-----------------------
		memset((char *)&addrOneAnalogIn, 0, sizeof(addrOneAnalogIn));
		inet_pton(AF_INET, EBU_IP_ANY, &(addrOneAnalogIn.sin_addr));
		addrOneAnalogIn.sin_port = htons(PORT_ANALOG_IN);
		//---------------------------------EBUDigitalIn for EBU 1------------------------
		memset((char *)&addrOneDigitalIn, 0, sizeof(addrOneDigitalIn));
		inet_pton(AF_INET,EBU_IP_ANY, &(addrOneDigitalIn.sin_addr));
		addrOneDigitalIn.sin_port = htons(PORT_DIGITAL_IN);
		//--------------------------------EBUAnalogOut for EBU 2------------------------
		memset((char *)&addrTwoAnalogOut, 0, sizeof(addrTwoAnalogOut));
		inet_pton(AF_INET, EBU_IP_2, &(addrTwoAnalogOut.sin_addr));
		addrTwoAnalogOut.sin_port = htons(PORT_ANALOG_OUT);
		//--------------------------------EBUDigitalOut for EBU 2-----------------------
		memset((char *)&addrTwoDigitalOut, 0, sizeof(addrTwoDigitalOut));
		inet_pton(AF_INET, EBU_IP_2, &(addrTwoDigitalOut.sin_addr));
		addrTwoDigitalOut.sin_port = htons(PORT_DIGITAL_OUT);
		//-------------------------------Relays for EBU 2--------------------------------------
		memset((char *)&addrTwoRelay, 0, sizeof(addrTwoRelay));
		inet_pton(AF_INET, EBU_IP_2, &(addrTwoRelay.sin_addr));
		addrTwoRelay.sin_port = htons(PORT_RELAYS);
		//-------------------------------EBUAnalogIn for EBU 2-----------------------
		memset((char *)&addrTwoAnalogIn, 0, sizeof(addrTwoAnalogIn));
		inet_pton(AF_INET, EBU_IP_ANY, &(addrTwoAnalogIn.sin_addr));
		addrTwoAnalogIn.sin_port = htons(PORT_ANALOG2_IN);
		//---------------------------------EBUDigitalIn for EBU 2------------------------
		memset((char *)&addrTwoDigitalIn, 0, sizeof(addrTwoDigitalIn));
		inet_pton(AF_INET,EBU_IP_ANY, &(addrTwoDigitalIn.sin_addr));
		addrTwoDigitalIn.sin_port = htons(PORT_DIGITAL2_IN);//change back to 2
		//------------------------------------------------------------------------------------------
		//------------------------------------Bind----------------------------------------------
		//------------------------------Bind for EBU 1------------------------------------
		if (bind(sockOneDigitalIn, (struct sockaddr *)&addrOneDigitalIn, sizeof(addrOneDigitalIn)) < 0) {return false;}
		if (bind(sockOneAnalogIn, (struct sockaddr *)&addrOneAnalogIn, sizeof(addrOneAnalogIn)) < 0) {return false;}
		//--------------------------------Bind for EBU 2-------------------------------------
		if (bind(sockTwoDigitalIn, (struct sockaddr *)&addrTwoDigitalIn, sizeof(addrTwoDigitalIn)) < 0) {return false;}
		if (bind(sockTwoAnalogIn, (struct sockaddr *)&addrTwoAnalogIn, sizeof(addrTwoAnalogIn)) < 0) {return false;}
		//-------------------------------------------------------------------------------------------------
		//-------------------------------Socket options------------------------------------------
		//-----------------------------Options for EBU 1----------------------------------------
		if (setsockopt(sockOneDigitalIn, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {return false;} //Timeout for recvfrom
		if (setsockopt(sockOneAnalogIn, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {return false;}
		//-----------------------------Options for EBU 2----------------------------------------
		if (setsockopt(sockTwoDigitalIn, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {return false;} //Timeout for recvfrom
		if (setsockopt(sockTwoAnalogIn, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0) {return false;}
		//--------------------------------------------------------------------------------------------
		return true; //If everything has been set up successfully return true.
	}catch (int e){
		perror("Connecting to EBU Error");
		throw e;
	}
}
//not complete, needs to handle errors, 4 functions not needed. maybe could make it into one or two similar to "send"
AnalogIn EBUManager::recvAnalogEBUOne() {
	char buffer[255];
	Packets::AnalogIn inData;
	struct sockaddr_in ebuAddr; //This struct stores the senders IP
	memset(&(ebuAddr.sin_zero), '\0', 8);
	if(recvfrom(sockOneAnalogIn, buffer, 255, 0, (struct sockaddr *)&ebuAddr, &slen)< 0){throw 14;}
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ebuAddr.sin_addr), str, INET_ADDRSTRLEN); //Gets senders IP into readable
	if((string)str == EBU_IP_1){inData.setSource(1);return inData;}	//Verifies that it's the correct sender IP
	inData.setSource(0); //0 means unvalid
	return inData;
}
AnalogIn EBUManager::recvAnalogEBUTwo() {
	char buffer[255];
	Packets::AnalogIn inData;
	struct sockaddr_in ebuAddr; //This struct stores the senders IP
	memset(&(ebuAddr.sin_zero), '\0', 8);
	if(recvfrom(sockTwoAnalogIn, buffer, 255, 0, (struct sockaddr *)&ebuAddr, &slen)< 0){throw 14;}
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ebuAddr.sin_addr), str, INET_ADDRSTRLEN); 	//Gets senders IP into readable
	if((string)str == EBU_IP_2){inData.setSource(2);return inData;}				//Verifies that it's the correct sender IP
	inData.setSource(0); //0 means unvalid
	return inData;
}
Packets::DigitalIn EBUManager::recvDigitalEBUOne() {
	char buffer[255];
	Packets::DigitalIn digidata;
	struct sockaddr_in ebuAddr;	//Stores senders IP
	memset(&(ebuAddr.sin_zero), '\0', 8);
	if(recvfrom(sockOneDigitalIn, buffer, 255, 0, (struct sockaddr *)&ebuAddr, &slen)<0){throw 14;}
	return digidata;
}
Packets::DigitalIn EBUManager::recvDigitalEBUTwo() {
	char buffer[255];
	Packets::DigitalIn digidata;
	struct sockaddr_in ebuAddr;	//Stores senders IP
	memset(&(ebuAddr.sin_zero), '\0', 8);
	if(recvfrom(sockTwoDigitalIn, buffer, 255, 0, (struct sockaddr *)&ebuAddr, &slen)<0){throw 14;}
	return digidata;
}

