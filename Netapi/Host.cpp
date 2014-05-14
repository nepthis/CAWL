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

Host::Host(char* a1, int p, char* a2, bool s) {
	addr1 = a1;
	addr2 = a2;
	port = p;
	isServer = s;
}

Host::~Host() {
	// Add cleanup, no biggy since not that many clients will be created
}


} /* namespace std */
