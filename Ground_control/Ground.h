/*
 * Ground.h
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */


#ifndef GROUND_H_
#define GROUND_H_

#define SND_PORT 56565
#define SND_ADDR "127.0.0.1"


#include <chrono>

#include <mutex>
#include <stdio.h>	//for printf
#include <string.h>	//std::string
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
#include <errno.h>	//For raising errors
#include <cstring>
//Own classes are included here
#include "../Simulator/Sim.h"
#include "../Packets/ImuPack.h"


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
	socklen_t slen;
	struct sockaddr_in grAddr;
	struct sockaddr_in recImuAddr;
	Packets::SimPack sp;
	Packets::SimPack state;
	Packets::ImuPack imuState;

	char *thetemp;
public:
	Simulator::Sim* simulator;
	Ground();
	void startRecieve();
	void startSend();
	void receiveImuPacket();
	void sendPacket();
	void receiveSimPack();
	virtual ~Ground();
};

#endif /* GROUND_H_ */
