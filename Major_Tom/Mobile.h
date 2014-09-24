/*
 * Mobile.h
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */

#ifndef MOBILE_H_
#define MOBILE_H_

#define REC_PORT 56565
#define IMU_PORT 45454
#define REC_ADDR "0.0.0.0"
#define GND_ADDR "192.168.2.100"
#include <mutex>
#include <chrono>
#include <cstdint> //Who doesn't like ints?
#include <string>	//Standard string
#include <errno.h>	//For a huge list of errors
#include <queue> //Used as a buffer
#include <unistd.h>
#include <netinet/in.h> //For UDP
#include <netdb.h> // in_addr_t
#include <iostream>

#include"../EBU/EBUManager.h"				//For communication to EBU
#include "../Simulator/Sim.h"
#include "../Packets/AnalogOut.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/AnalogIn.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/DigitalOut.h"	//In order to set which relays easier, it contains all defines.
#include "../Packets/DigitalIn.h"	//In order to set which relays easier, it contains all defines.
#include "../Packets/ImuPack.h"
#include "../Packets/RelayOut.h"
//#include "../Globals.h"


namespace Major_Tom {
/*	This class is responsible for the communication between the wheel loader's EBUs and the to the Ground running
 * 	with the simulator. In this iteration of the project we simply use UDP Sockets so that WebRTC can handle the connection
 * 	over internet from macbook - macbook.
 *		The IMU will be connected to the gateway running this mode of the application and the IMU data will be sent from here to
 *		Ground.
 */

class Mobile {
public:
	bool pleased;
	Mobile();	//Constructor
	void socketReceive(); 				//Receiving data from an UDP socket, port 65656
	void socketSend(); 					//Sending data back through socket
	void ebuSend(); 							//Send data to the EBU
	void imuRec();
	virtual ~Mobile();							//Destructor
	EBU::EBUManager em;
	Packets::RelayOut rPackOne;
	Packets::RelayOut rPackTwo;
private:
	EBU::EBUTranslator et;	//Translates simdata for the EBUs
	socklen_t slen;
	int mobSocket;	//Socket for mobile client, will listen for packages on port 56565
	int sndImuSocket;	//Socket for mobile client, will send packets on port 45454
	struct sockaddr_in mobAddr;
	struct sockaddr_in sndImuAddr;
	Packets::AnalogOut stopPacket;
	Packets::SimPack state;
};

} /* namespace Major_Tom */

#endif /* MOBILE_H_ */
