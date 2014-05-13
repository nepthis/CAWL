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
#include <stdio.h>

#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../Packets/EBUPacketAnalogOut.h"
#include "../Packets/EBURelayPacket.h"

#ifndef EBUMANAGER_H_
#define EBUMANAGER_H_

namespace EBU{

class EBUManager {
	socklen_t slen;
	//int oneAnalogIn;
	int oneAnalogOut;
	int oneRelay;

	int destinationPort;

	Packets::EBURelayPacket relayPack;
	//struct sockaddr_in addrOneAnalogIn; Port 25101, Analog data FROM the EBU
	struct sockaddr_in addrOneAnalogOut;//Port 25200, data TO the EBU
	struct sockaddr_in addrOneRelay; //Port 25400, send relay data here
//	struct sockaddr_in addrTwoAnalogIn;
//	struct sockaddr_in addrTwoAnalogOut;
public:
	EBUManager();
	virtual ~EBUManager();
	//ebuNum (1 = ebu one, 2 = ebutwo)
	int sendAnalogCommand(Packets::EBUPacketAnalogOut, int ebuNum);
	int sendRelayCommand(Packets::EBURelayPacket, int ebuNum);

};
}
#endif /* EBUMANAGER_H_ */
