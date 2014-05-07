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

EBUManager::EBUManager() {
	// Constructor
	if ((ebuSockOne = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");

	}if ((ebuSockTwo = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");

	}
}

EBUManager::~EBUManager() {
	// TODO Auto-generated destructor stub
}

//Hmm, kan vara onödig.
double EBUManager::convertVoltToBit(int voltIn)
{
	double voltOut = voltIn;

	return voltOut;
}
int EBUManager::startConnection(int ebuNum){
	if (ebuNum == 1){
		memset((char *)&ebuOne, 0, sizeof(ebuOne));
//		ebuOne.sin_family = AF_INET;
//		ebuOne.sin_addr.s_addr = htonl('10.10.0.1');
		inet_pton(AF_INET, "10.10.0.1", &(ebuOne.sin_addr)); //Lättare att använda
		ebuOne.sin_port = htons(0); //Add right port...
		if (bind(ebuSockOne, (struct sockaddr *)&ebuOne, sizeof(ebuOne)) < 0) {
			perror("error with bind");
			return -1;
		}
		return 1;
	}
	if(ebuNum == 2){
		memset((char *)&ebuTwo, 0, sizeof(ebuTwo));
		//ebuTwo.sin_family = AF_INET;
		inet_pton(AF_INET, "10.10.0.2", &(ebuTwo.sin_addr)); //Lättare att använda
		//ebuTwo.sin_addr.s_addr = htonl(address);
		ebuTwo.sin_port = htons(0);//PORRTTTSSSS
		if (bind(ebuSockOne, (struct sockaddr *)&ebuTwo, sizeof(ebuTwo)) < 0) {
			perror("error with bind");
			return -1;
		}
		return 1;
	}

	return -1; //Failed

}
int EBUManager::sendCommand(EBUPacketAnalogIn p){
	dataToSend data = p.getDataToSend();
//	if (sendto(ebuSockOne)){
//
//	}

	//delete packet after sent, maybe.
	return 1; //if success
}
int EBUManager::sendCommand(EBUPacketAnalogOut p){
	return 1; //if success
}
/*
int EBUManager::readData(uint16_t pin, uint32_t value){

	return 1; //if success
}
*/

