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
	double sentPackages, recPackages;
	struct sockaddr_in ebuOne;
	struct sockaddr_in ebuTwo;
public:
	EBUManager();
	virtual ~EBUManager();
	double convertVoltToBit(int volt);
	int startConnection(int ebuNum, char addrEbu);
	//Should probably not be void, but for now they are.
	int sendCommand(uint16_t pin, uint32_t value);
	int readData(uint16_t pin, uint32_t value);
};

#endif /* EBUMANAGER_H_ */
