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
		Host(char* a1,int p, char* a2=(char*)"0.0.0.0", bool s=false);
		virtual ~Host();

		char * addr1;
		char * addr2;
		int  port;
		bool isServer;

};

} /* namespace std */

#endif /* HOST_H_ */
