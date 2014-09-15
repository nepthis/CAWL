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
#define EBU_IP_1 "10.0.0.2"
#define EBU_IP_2 "10.0.0.3"
#define EBU_IP_ANY "0.0.0.0"
#define PORT_ANALOG_OUT 25200
#define PORT_ANALOG_IN 25201
#define PORT_DIGITAL_OUT 25300
#define PORT_DIGITAL_IN 25301
#define PORT_RELAYS 25400


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
 *		It can be used for both reading and sending data to the EBUs. The translation of
 *		packets from the simulator is also performed here.
 */

class EBUManager {
	socklen_t slen;
	//---------------------------------------------------------------------------------------------------------------------------------------
	int oneAnalogOut;	//Sockets for the AnalogOut Packages TO the EBU
	int twoAnalogOut;
	int oneAnalogIn;
	int twoAnalogIn;
	int oneDigitalOut;	//Sockets for sending DigitalPackets to the EBU
	int twoDigitalOut;
	int oneDigitalIn;		//Sockets for reading digital data from the EBU
	int twoDigitalIn;
	int oneRelay;				//Sockets for sending relay Packages to the EBU
	int twoRelay;
	//---------------------------------------------------------------------------------------------------------------------------------------
	Packets::EBURelayPacket relayPack;
	//---------------------------------------------------------------------------------------------------------------------------------------
	struct sockaddr_in addrOneAnalogOut;	//Port 25200, data TO the EBU
	struct sockaddr_in addrTwoAnalogOut;
	struct sockaddr_in addrOneAnalogIn;		//Port 25101, Analog data FROM the EBU
	struct sockaddr_in addrTwoAnalogIn;
	struct sockaddr_in addrOneDigitalOut;		//Port 25300 for sending digital to the EBU
	struct sockaddr_in addrTwoDigitalOut;
	struct sockaddr_in addrOneDigitalIn; 		//Port 25301 for reading digital info
	struct sockaddr_in addrTwoDigitalIn;
	struct sockaddr_in addrOneRelay; 				//Port 25400, send relay data here
	struct sockaddr_in addrTwoRelay;
	//---------------------------------------------------------------------------------------------------------------------------------------
public:
	EBUManager();
	virtual ~EBUManager();
	//ebuNum (1 = ebu one, 2 = ebutwo)
	bool connectToEBU();
	void sendAnalogCommand(Packets::ebuAnOut data, int ebuNum);
	void sendDigitalCommand(Packets::EBUdigitalOut data, int ebuNum);
	void sendRelayCommand(Packets::EBURelayPacket, int ebuNum);
	void recAnalogIn(int ebuNum);
	void recDigitalIn(int ebuNum);
};
}
#endif /* EBUMANAGER_H_ */
