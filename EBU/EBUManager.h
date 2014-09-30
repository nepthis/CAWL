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


#include <cstring> //Allows the usage of std::strings
#include <string> //Allows the usage of std::strings
#include <cstdint> //To access all types of ints
#include <stdlib.h>
#include <stdio.h> //Used for printf
#include <sys/socket.h> //Used for the UDP socket to the EBU
#include <errno.h>		//Is used for raising errors
#include <netinet/in.h>	//Network address structs
#include <arpa/inet.h>	//defenitions for internet options

#include "../Packets/AnalogOut.h"	//Packet for sending data TO the EBU
#include "../Packets/AnalogIn.h"
#include "../Packets/DigitalOut.h"
#include "../Packets/DigitalIn.h"
#include "../Packets/RelayOut.h"		//Packet for sending information on which relays to be open


namespace EBU{

#define EBU_IP_1 "10.0.0.2"
#define EBU_IP_2 "10.0.0.3"
#define EBU_IP_ANY "0.0.0.0"
#define PORT_ANALOG_OUT 25200
#define PORT_ANALOG_IN 25101
#define PORT_DIGITAL_OUT 25300
#define PORT_DIGITAL_IN 25301
#define PORT_RELAYS 25400

/*	This class will maintain the connection to the EBUs (some functions require both EBUs)
 *		It can be used for both reading and sending data to the EBUs. The translation of
 *		packets from the simulator is also performed here.
 */
class EBUManager {
	socklen_t slen;
	//---------------------------------------------------------------------------------------------------------------------------------------
	int oneAnalogOut;	//Sockets for the AnalogOut Packages TO the EBU
	int twoAnalogOut;
	int oneDigitalOut;	//Sockets for sending DigitalPackets to the EBU
	int twoDigitalOut;
	int sockDigitalIn;		//Sockets for reading digital data from the EBU
	int sockAnalogIn;
	int oneRelay;				//Sockets for sending relay Packages to the EBU
	int twoRelay;
	//---------------------------------------------------------------------------------------------------------------------------------------
	Packets::RelayOut relayPack;
	//---------------------------------------------------------------------------------------------------------------------------------------
	struct sockaddr_in addrOneAnalogOut;	//Port 25200, data TO the EBU
	struct sockaddr_in addrTwoAnalogOut;
	struct sockaddr_in addrOneDigitalOut;		//Port 25300 for sending digital to the EBU
	struct sockaddr_in addrTwoDigitalOut;
	struct sockaddr_in addrDigitalIn; 		//Port 25301 for reading digital info
	struct sockaddr_in addrAnalogIn;		//Port 25101, Analog data FROM the EBU
	struct sockaddr_in addrOneRelay; 				//Port 25400, send relay data here
	struct sockaddr_in addrTwoRelay;
	//---------------------------------------------------------------------------------------------------------------------------------------
public:
	EBUManager();
	virtual ~EBUManager();
	bool connectToEBU();
	void sendAnalogCommand(Packets::ebuAnOut data, int ebuNum);
	void sendDigitalCommand(Packets::EBUdigitalOut data, int ebuNum);
	void sendRelayCommand(Packets::RelayOut, int ebuNum);
	Packets::AnalogIn recAnalogIn();
	Packets::DigitalIn recDigitalIn();
};
}
#endif /* EBUMANAGER_H_ */
