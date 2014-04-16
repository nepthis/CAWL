/*
 * Sim.h
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-server
 */
#include <sys/socket.h>
#include <string.h>
#include <../Packets/DataPacket.h>
#include <time.h>
using namespace std;

#ifndef SIM_H_
#define SIM_H_

class Sim {
	struct socket_in socket_out;
	string address;
	int port;
	double sentPackages, recPackages;
	time_t startTime;
public:
	Sim();
	virtual ~Sim();
	void startConnection (string address, int port);
	void closeConnection(void);
	void recPac(void);
	void sendPac(header hdr, data dt);
};


#endif /* SIM_H_ */
