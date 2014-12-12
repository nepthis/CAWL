/*
 * Mobile.h
 *  Created on: Jun 23, 2014
 *  Author: Robin Bond & H�kan Ther�n
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */

#ifndef MOBILE_H_
#define MOBILE_H_

#define REC_PORT 56565
#define IMU_PORT 45454
#define REC_ADDR "0.0.0.0"
#define GND_ADDR "192.168.2.100"
#define DESTI_ADDR "192.168.2.100"
//"10.0.0.199"

#include <mutex>
#include <chrono>
#include <cstdint> //Who doesn't like ints?
#include <string>	//Standard string
#include <errno.h>	//For a huge list of errors
#include <unistd.h>
#include <netinet/in.h> //For UDP
#include <netdb.h> // in_addr_t
#include <iostream>

#include"EBUManager.h"
#include "../Packets/AllPackets.h"
#include "EBUTranslator.h"
#include "IMUManager.h"

#include "../Globals.h"


namespace Major_Tom {
/*	This class is responsible for the communication between the wheel loader's EBUs and the to the Ground running
 * 	with the simulator. It provides methods to be started as threads for receiving data from Ground, sending imu
 * 	data to Ground and to send data to the EBUs.
 */

class Mobile {
public:
	Mobile(bool sctp);	//Constructor
	bool startUp();
	void recvGround(); 				//Receiving data from an UDP socket, port 65656
	void sendEBUOne();
	void sendEBUTwo();
	void recvEBUOne();
	void recvEBUTwo();	//Kind of, sendEBUX actualy does the receiving in order to sync data with EBUs...
	void recvFromIMU();
	void sendIMU();
	void setSCTP();
	void sendAllStop();
	virtual ~Mobile();							//Destructor
	Packets::RelayOut rPackOne;
	Packets::RelayOut rPackTwo;
	EBU::EBUManager em;	//Manages EBU connections
private:
	IMUManager imm;
	Packets::ImuPack imuState;
	int errors;
	bool sctpIsOn;		//if set to true sctp will be used instead of udp.

	EBU::EBUTranslator et;	//Translates simdata for the EBUs
	socklen_t slen;
	int mobSocket;	//Socket for mobile client, will listen for packages on port 56565
	int sndImuSocket;	//Socket for mobile client, will send packets on port 45454
	struct sockaddr_in mobAddr;
	struct sockaddr_in sndImuAddr;
	Packets::AnalogOut stopPacket;
	Packets::SimPack state;
	Packets::AnalogIn recvAnalogOne;
	Packets::AnalogIn recvAnalogTwo;
	Packets::DigitalIn recvDigitalOne;
	Packets::DigitalIn recvDigitalTwo;
};

} /* namespace Major_Tom */

#endif /* MOBILE_H_ */
