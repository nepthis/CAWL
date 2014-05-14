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
#include "../Packets/CawlPacket.h"


#ifndef CAWLSOCKET_H_
#define CAWLSOCKET_H_

#define RECVBUFSIZE	256
#define PPID 1234

namespace Netapi{

class CawlSocket {
private:
	int SctpScocket, n, flags;
	socklen_t from_len;

	//forget about memset(,0,)
	struct sockaddr_in addr;
	struct sctp_sndrcvinfo sinfo;
	struct sctp_event_subscribe event;
	struct sctp_initmsg initmsg;
	char pRecvBuffer[RECVBUFSIZE + 1];

	bool isServer;

public:
	CawlSocket(Host& h);
	// Cawlpacket needs to be changed from std to Net
	void send(Packets::CawlPacket& p);
	void rec(Packets::CawlPacket& p);

	virtual ~CawlSocket();

};
}
/* namespace Net */
#endif /* CAWLSOCKET_H_ */
