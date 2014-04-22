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

Host::Host(char ip, int port, bool server) {
	// TODO Auto-generated constructor stub
	SetHost(ip, port, server);
}

Host::~Host() {
	// TODO Auto-generated destructor stub
	//
	// Add cleanup, no biggy since not that many clients will be created
}

void Host::SetHost(char ip, int port, bool server) {
	o_ip = ip;
	o_port = port;
	o_server = server;
}

} /* namespace std */
