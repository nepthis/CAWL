/*
 * MPManager.h
 *
 *  Created on: Dec 9, 2014
 *      Author: cawl
 */

#ifndef IMU_MPMANAGER_H_
#define IMU_MPMANAGER_H_

#include <stdlib.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <errno.h>

#include "../logger.h"
#include "../Packets/ImuPack.h"

//SIM SEND PARAMS
#define SIM_ADDR "192.168.2.97"
#define SIM_PORTEN 12345
#define SIM_FREQ 100


namespace IMU{

class MPManager{

public:
	MPManager();//bool imu_rec, bool sim_snd
	virtual ~MPManager();
	void sendSim(Packets::ImuPack imu); //to simulator

	//Used for sending to sim
	int simsock;
	struct sockaddr_in simAddr;

private:
	void setupSockets();

};
}

#endif /* IMU_MPMANAGER_H_ */
