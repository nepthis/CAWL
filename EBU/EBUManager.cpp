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
}
EBUManager::~EBUManager() {
}
/*	Sends an the data-struct inside the analogPacket to the EBU. A switch statements will send the packet either
 * 	to EBU no. 1 or EBU no. 2. The destination EBU is set when creating the packet. Other than that it's a
 * 	regular sendto with UDP
 */
void EBUManager::sendAnalogCommand(ebuAnOut data, int ebuNum){

	try{
		switch(ebuNum){

		case 1:
			sendto(oneAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneAnalogOut, slen);
			break;
		case 2:
			sendto(twoAnalogOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoAnalogOut, slen);
			break;
		/* Håkan kommenterade ut. Som jag ser det får man oönskad effekt
		default:
			AnalogOut epao;
			ebuAnOut eo = epao.getChannel();
			sendto(oneAnalogOut, (char*)&eo, sizeof(eo), 0, (struct sockaddr*) &addrOneAnalogOut, slen);
			sendto(twoAnalogOut, (char*)&eo, sizeof(eo), 0, (struct sockaddr*) &addrTwoAnalogOut, slen);
			break;
		*/
		}
	}catch(int e){
		printf("Error number: %i\n",e);
		perror("Error sending Aanalog Packet to the EBU\n");
		throw 1; //TBD
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
	try{
		switch(ebuNum){
		case 1:
			sendto(oneRelay,buffer, 14, 0, (struct sockaddr*) &addrOneRelay, slen);
			break;
		case 2:
			sendto(twoRelay, buffer, 14, 0, (struct sockaddr*) &addrTwoRelay, slen);
			break;
		default:
			RelayOut ro;
			data = ro.getRelays();
			memcpy(buffer, &data, 14);
			ro.destination = 1;
			sendto(oneRelay,buffer, 14, 0, (struct sockaddr*) &addrOneRelay, slen);
			ro.destination = 2;
			sendto(twoRelay, buffer, 14, 0, (struct sockaddr*) &addrTwoRelay, slen);
			break;
		}
	}catch(int e){
		printf("Error number: %i\n",e);
		perror("Error sending relay packet to an EBU\n");
		throw 1; //TBD
	}
}

void EBU::EBUManager::sendDigitalCommand(EBUdigitalOut data, int ebuNum) {
	try{
		switch(ebuNum){
		case 1:
			sendto(oneDigitalOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrOneDigitalOut, slen);
			break;
		case 2:
			sendto(twoDigitalOut, (char*)&data, sizeof(data), 0, (struct sockaddr*) &addrTwoDigitalOut, slen);
			break;
		default:
			DigitalOut epd;
			EBUdigitalOut ed = epd.getChannel();
			sendto(oneDigitalOut, (char*)&ed, sizeof(ed), 0, (struct sockaddr*) &addrOneDigitalOut, slen);
			sendto(twoDigitalOut, (char*)&ed, sizeof(ed), 0, (struct sockaddr*) &addrTwoDigitalOut, slen);
			break;
		}
	}catch(int e){
		printf("Error number: %i\n",e);
		perror("Error sending Analog Packet to the EBU\n");
		throw 1; //TBD
	}
}
//Read this at your own risk.
bool EBU::EBUManager::connectToEBU() {
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
	if ((sockAnalogIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{perror("socket error");printf ("Error number is: %s\n",strerror(errno));return false;}
	//---------------------------------DigitalIn-----------------------------------------------
	if ((sockDigitalIn = socket(AF_INET,SOCK_DGRAM,0)) < 0)
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
	//--------------------------------------------------------------------------------------------
	//-------------------------------EBUAnalogIn for EBUs-----------------------
	memset((char *)&addrAnalogIn, 0, sizeof(addrAnalogIn));
	inet_pton(AF_INET, EBU_IP_ANY, &(addrAnalogIn.sin_addr));
	addrAnalogIn.sin_port = htons(PORT_ANALOG_IN);
	//---------------------------------EBUDigitalIn for EBUs------------------------
	memset((char *)&addrDigitalIn, 0, sizeof(addrDigitalIn));
	inet_pton(AF_INET,EBU_IP_ANY, &(addrDigitalIn.sin_addr));
	addrDigitalIn.sin_port = htons(PORT_DIGITAL_IN);
	//--------------------------------------------------------------------------------------------
	//-------------------------------Bind for EBU 1 & 2--------------------------------------
	if (bind(sockDigitalIn, (struct sockaddr *)&addrDigitalIn, sizeof(addrDigitalIn)) < 0) {return false;}
	if (bind(sockAnalogIn, (struct sockaddr *)&addrAnalogIn, sizeof(addrAnalogIn)) < 0) {return false;}
	return true;
}
//not complete, needs to handle errors
AnalogIn EBUManager::recAnalogIn() {
	char buffer[255];
	Packets::AnalogIn inData;
	struct sockaddr_in ebuAddr; //This struct stores the senders IP
	memset(&(ebuAddr.sin_zero), '\0', 8);
	recvfrom(sockAnalogIn, buffer, 255, 0, (struct sockaddr *)&ebuAddr, &slen);
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ebuAddr.sin_addr), str, INET_ADDRSTRLEN); //Gets senders IP into readable
	if((string)str == "10.0.0.2"){inData.setSource(1);return inData;}
	if((string)str == "10.0.0.3"){inData.setSource(2);return inData;}
	inData.setSource(0); //0 means unvalid
	return inData;
}

Packets::DigitalIn EBUManager::recDigitalIn() {
	char buffer[255];
	Packets::DigitalIn digidata;
	struct sockaddr_in ebuAddr;	//Stores senders IP
	memset(&(ebuAddr.sin_zero), '\0', 8);
	recvfrom(sockDigitalIn, buffer, 255, 0, (struct sockaddr *)&ebuAddr, &slen);
	char str[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(ebuAddr.sin_addr), str, INET_ADDRSTRLEN); //Extract senders IP into str, readable.
	if((string)str =="10.0.0.2"){digidata.setSource(1);return digidata;}
	if((string)str == "10.0.0.3"){digidata.setSource(2);return digidata;}
	digidata.setSource(0); //0 means unvalid
	return digidata;
}
