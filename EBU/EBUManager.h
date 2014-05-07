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


#ifndef EBUMANAGER_H_
#define EBUMANAGER_H_

namespace EBU{

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
	double convertVoltToBit(int);
	int startConnection(int);
	int initRelay(void);
	int sendCommand(Packets::EBUPacketAnalogIn);
	int sendCommand(EBUPacketAnalogOut);
	//int readData(EBUPacketAnalogOut);
};
}
#endif /* EBUMANAGER_H_ */
