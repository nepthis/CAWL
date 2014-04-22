/*
 * Host.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: Hakan Theren
 *
 * Description: Class for holding host variables such as ip etc.
 */

#include "Host.h"

namespace std {

Host::Host(char addr, int port, bool server) {
	SetHost(addr, port, server);
}

Host::~Host() {
	// Add cleanup, no biggy since not that many clients will be created
}

void Host::SetHost(char addr, int port, bool server) {
	o_addr = addr;
	o_port = port;
	o_server = server;
}

} /* namespace std */
