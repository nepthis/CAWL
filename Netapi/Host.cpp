/*
 * Host.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: Hakan Theren
 *
 * Description: Class for holding host variables such as ip etc.
 */

#include "Host.h"

namespace Netapi {

Host::Host(std::string a1 ,int port ,bool isServer ,std::string a2) {
	addr1 = a1;
	addr2 = a2;
	o_port = port;
	o_isServer = isServer;
}

Host::~Host() {
	// Add cleanup, no biggy since not that many clients will be created
}


} /* namespace std */
