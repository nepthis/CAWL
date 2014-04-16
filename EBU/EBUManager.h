/*
 * EBUManager.h
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-server
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
	void setEbuAddr(int ebuNum, string addr);
	//Should probably not be void, but for now they are.
	void sendCommand(uint16_t pin, uint32_t value);
	void readData(uint16_t pin, uint32_t value);
};

#endif /* EBUMANAGER_H_ */
