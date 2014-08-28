/*
 * EBUManager.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 * Description:
 * This class will be responsible for taking a command and sending it to the right
 * EBU with the values in the right format.
 */
#ifndef EBUMANAGER_H_
#define EBUMANAGER_H_
#include <string.h> //Allows the usage of std::strings
#include <stdint.h> //To access all types of ints
#include <stdlib.h>
#include <stdio.h> //Used for printf

#include <sys/socket.h> //Used for the UDP socket to the EBU
#include <errno.h>		//Is used for raising errors
#include <netinet/in.h>	//Network address structs
#include <arpa/inet.h>	//defenitions for internet options

#include "../Packets/EBUPacketAnalogOut.h"	//Packet for sending data TO the EBU
#include "../Packets/EBUPacketDigitalOut.h"
#include "../Packets/EBURelayPacket.h"		//Packet for sending information on which relays to be open
namespace EBU{
/*	This class will maintain the connection to the EBUs (some functions require both EBUs)
 *	More functions should be added when they will be implemented, mostly functions for
 *	reading packages from the EBU and sending digital packages.
 *	When e.g sending an analogPacket to the EBU simply use the sendAnalogCommand function
 *	with an analogout packet + destination as arguments. The destination is set in the
 *	packet for easy access.
 */

class EBUManager {
	socklen_t slen;
	int oneAnalogOut;	//Sockets for the AnalogOut Packages TO the EBU
	int twoAnalogOut;
	int oneDigitalOut;	//Sockets for sending DigitalPackets to the EBU
	int twoDigitalOut;
	int oneRelay;				//Sockets for sending relay Packages to the EBU
	int twoRelay;
	Packets::EBURelayPacket relayPack;
	//struct sockaddr_out addrOneAnalogIn; Port 25101, Analog data FROM the EBU
	struct sockaddr_in addrOneAnalogOut;	//Port 25200, data TO the EBU
	struct sockaddr_in addrTwoAnalogOut;
	struct sockaddr_in addrOneDigitalOut;		//Port 25300 for sending digital to the EBU
	struct sockaddr_in addrTwoDigitalOut;
	struct sockaddr_in addrOneRelay; //Port 25400, send relay data here
	struct sockaddr_in addrTwoRelay;

public:
	EBUManager();
	virtual ~EBUManager();
	//ebuNum (1 = ebu one, 2 = ebutwo)
	void sendAnalogCommand(Packets::ebuAnOut data, int ebuNum);
	void sendDigitalCommand(Packets::EBUdigitalOut data, int ebuNum);
	void sendRelayCommand(Packets::EBURelayPacket, int ebuNum);

};
}
#endif /* EBUMANAGER_H_ */
