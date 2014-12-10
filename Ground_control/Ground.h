/*
 * Ground.h
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */


#ifndef GROUND_H_
#define GROUND_H_

#define DEST_PORT 56565
#define REC_IMU_PORT 45454
#define DEST_ADDR	"192.168.2.199"
#define RECVBUFSIZE	255


#include <chrono>

#include <mutex>
#include <stdio.h>	//for printf
#include <string.h>	//std::string
#include <ctype.h>
#include <cmath> //for fabs
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <errno.h>	//For raising errors
#include <cstring>
//Own classes are included here
#include "Sim.h"
#include "../Packets/AllPackets.h"
#include "../IMU/IMUManager.h"
#include "../logger.h"


namespace Ground_control {
/*	The class ground is responsible for maintaining a connection with the simulator (sim.h).
 * 	The methods startReceive and startSend are started are made for being started in threads
 * 	and will keep on receiving packages from the simulator and sending them over the cawlsocket or an UDP socket.
 */
class Ground {
private:
	IMU::IMUManager im;
	//----------------Common stuff--------------------
	int grSocket;
	int recImuSocket;
	bool sctpIsOn;
	socklen_t slen;
	//------------------UDP------------------
	struct sockaddr_in grAddr;
	struct sockaddr_in recImuAddr;
	Packets::SimPack sp;
	Packets::SimPack state;
	Packets::ImuPack imuStateToSim;
	//----------------------SCTP---------------------
//	char pRecvBuffer[RECVBUFSIZE + 1];
//	struct sockaddr_in addrSCTP;
//	struct sctp_sndrcvinfo sinfoSCTP;
//	struct sctp_event_subscribe eventSCTP;
//	char * szAddress;
//	int sctpPort;
//	char * szMsg;
//	int iMsgSize;

public:
	Ground_control::Sim* simulator;
	Ground(bool sctpStatus);
	void receiveImuPacket();
	void sendMobile();
	void sendSim();
	void receiveSim();
	virtual ~Ground();
};
}
#endif /* GROUND_H_ */
