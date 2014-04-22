/*
 * Server.h
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-mobile
 */

#include "Host.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <arpa/inet.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <syslog.h>

#ifndef SERVER_H_
#define SERVER_H_

#define RECVBUFSIZE             4096

using namespace std;

class Server {
private:


public:
	Server(Host c);

	virtual ~Server();

};

#endif /* SERVER_H_ */
