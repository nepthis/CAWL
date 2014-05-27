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

#include <cstring>
#include <utility>
#include <stdint.h>


namespace Netapi {

typedef struct Host{
	char * addr1;
	char * addr2;
	int  port;
	bool isServer;
	Host(char* addr_server,int port, char* addr_bind=(char*)"0.0.0.0", bool is_server=false);

	void SetServerAddr(char * server_addr);
	void SetBindAddr(char * bind_addr);

	char* GetServerAddr(){return addr1;};
	char* GetBindAddr(){return addr2;};

	void SetPort(int Port){port = Port;}
	void SetServer(bool Server){isServer = Server;}

	int GetPort(){return port;}
	bool GetServer(){return isServer;}

	virtual ~Host();
}Host;

} /* namespace std */

#endif /* HOST_H_ */
