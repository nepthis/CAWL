/*
 * Host.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Hakan Theren
 *
 * Description: Class for holding host variables such as ip etc.
 */

#ifndef HOST_H_
#define HOST_H_

namespace std {

class Host {

	private:
		char o_ip;
		int  o_port;
		bool o_server;

	public:
		Host(char ip , int port , bool server);
		virtual ~Host();

		void SetHost(char ip, int port, bool server);

		char GetIp()	{return o_ip;}
		int  GetPort()	{return o_port;}
		bool GetServer(){return o_server;}

};

} /* namespace std */

#endif /* HOST_H_ */
