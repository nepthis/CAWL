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

#ifndef HOST_H_
#define HOST_H_

namespace Netapi {

class Host {

	private:
		std::string addr1;
		std::string addr2;
		int  o_port;
		bool o_isServer;

	public:
		Host(std::string, int, bool = false, std::string = "0.0.0.0");
		virtual ~Host();

		std::pair<std::string,std::string> getAddr(){return std::make_pair(addr1,addr2);}
		int  getPort(){return o_port;}
		bool getServer(){return o_isServer;}
		};

} /* namespace std */

#endif /* HOST_H_ */
