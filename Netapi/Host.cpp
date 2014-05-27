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

Host::Host() {
	addr1 = (char*)"0.0.0.0";
	addr2 = (char*)"0.0.0.0";
	port = 1;
	isServer = false;
}

Host::Host(char* a1, int p, char* a2, bool s) {
	addr1 = a1;
	addr2 = a2;
	port = p;
	isServer = s;
}

void Host::SetServerAddr(char * server_addr){
	addr1 = server_addr;
}
void Host::SetBindAddr(char * bind_addr){
	addr2 = bind_addr;
}

Host::~Host() {
}


} /* namespace std */
