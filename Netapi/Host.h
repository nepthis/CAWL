/*
 * Host.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Hakan Theren
 *
 * Description: Class for holding host variables such as ip etc.
 */

#include <string>
#include <utility>
#include <stdint.h>

#ifndef HOST_H_
#define HOST_H_

namespace Netapi {

class Host {
	public:
		Host(char* addr_server,int port, char* addr_bind=(char*)"0.0.0.0", bool is_server=false);
		virtual ~Host();

		char * addr1;
		char * addr2;
		int  port;
		bool isServer;

};

} /* namespace std */

#endif /* HOST_H_ */
