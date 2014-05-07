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
#include "CawlPacket.h"


#ifndef CAWLSOCKET_H_
#define CAWLSOCKET_H_

#define RECVBUFSIZE	256

namespace Netapi{

class CawlSocket {
private:
	int SctpScocket, n, flags;
	socklen_t from_len;

	//forget about memset(,0,)
	struct sockaddr_in addr = {0};
	struct sctp_sndrcvinfo sinfo = {0};
	struct sctp_event_subscribe event = {0};
	char pRecvBuffer[RECVBUFSIZE + 1] = {0};

	char * szAddress;
	int iPort;
	char * szMsg;
	int iMsgSize;
	int isServer;

public:
	int CawlSocket();
	// Cawlpacket needs to be changed from std to Net
	int send(Packets::CawlPacket*);
	int rec(Packets::CawlPacket*);
	int bind(Host*);

	virtual ~CawlSocket();

};
}
/* namespace Net */
#endif /* CAWLSOCKET_H_ */

