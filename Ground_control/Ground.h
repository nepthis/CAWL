/*
 * Ground.h
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */


#ifndef GROUND_H_
#define GROUND_H_

#define DEST_PORT 56565
#define DEST_ADDR "192.168.2.199"


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


namespace Ground_control {
/*	The class ground is responsible for maintaining a connection with the simulator (sim.h)
 * 	and to translate packages from the simulator into packets for the EBU and then to send
 * 	them to the Mobile gateway using a CawlSocket.
 * 	The methods startReceive and startSend are started are made for being started in threads
 * 	and will keep on receiving packages from the simulator and sending them over the cawlsocket.
 */
class Ground {
private:
	int grSocket;
	int recImuSocket;
	bool sctpIsOn;
	socklen_t slen;
	struct sockaddr_in grAddr;
	struct sockaddr_in recImuAddr;
	Packets::SimPack sp;
	Packets::SimPack state;
	Packets::ImuPack imuState;

public:
	Ground_control::Sim* simulator;
	Ground(bool sctpStatus);
	void receiveImuPacket();
	void sendMobile();
	void receiveSim();
	virtual ~Ground();
};
}
#endif /* GROUND_H_ */
