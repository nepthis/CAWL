/*
 * Server.h
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-mobile
 */

#include "Host.h"

#ifndef SERVER_H_
#define SERVER_H_

 using namespace std;

class Server {
public:
	Server(Host c);
	virtual ~Server();

	void SendMsg(char c);

};

#endif /* SERVER_H_ */
