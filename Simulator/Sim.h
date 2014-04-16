/*
 * Sim.h
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-server
 */
#include <sys/socket.h>
#ifndef SIM_H_
#define SIM_H_

class Sim {
	struct socket_in socket_out;
public:
	Sim();
	virtual ~Sim();
};

#endif /* SIM_H_ */
