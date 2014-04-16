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
	string ebuAddrOne, ebuAddrTwo;
public:
	EBUManager();
	virtual ~EBUManager();
	void
	void setEbuAddr(int ebuNum, string addr);
	//Should probably not be void, but for now they are.
	void sendCommand(uint16_t pin, uint32_t value);
	void readData(uint16_t pin, uint32_t value);
};

#endif /* EBUMANAGER_H_ */
