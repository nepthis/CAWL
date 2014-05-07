/*
 * EBUManager.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 * Description:
 * This class will be responsible for taking a command and sending it to the right
 * EBU with the values in the right format.
 */
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/socket.h>

using namespace std;

#ifndef EBUMANAGER_H_
#define EBUMANAGER_H_

class EBUManager {
	int ebuSockOne;
	int ebuSockTwo;
	uint16_t analog[24];
	uint16_t digital[24];
	struct sockaddr_in ebuOne;
	struct sockaddr_in ebuTwo;
public:
	EBUManager();
	virtual ~EBUManager();
	double convertVoltToBit(int volt);
	int startConnection(int ebuNum);
	int initRelay(void);
	int sendCommand(EBUPacketAnalogIn p);
	int sendCommand(EBUPacketAnalogOut p);
	//int readData(EBUPacketAnalogOut p);
};

#endif /* EBUMANAGER_H_ */
