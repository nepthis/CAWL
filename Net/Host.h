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
		char o_addr;
		int  o_port;
		bool o_server;

	public:
		Host(char addr , int port , bool server);
		virtual ~Host();

		void SetHost(char addr, int port, bool server);

		char GetAddr()	{return o_addr;}
		int  GetPort()	{return o_port;}
		bool GetServer(){return o_server;}

};

} /* namespace std */

#endif /* HOST_H_ */
