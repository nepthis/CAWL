/*
 * CawlSocket.cpp
 *
 *  Created on: May 7, 2014
 *      Author: cawl-mobile
 */

#include "CawlSocket.h"
#include "../Netapi/Host.h"
#include "../Packets/CawlPacket.h"

namespace Netapi {

CawlSocket::CawlSocket(Netapi::Host& h) {
	n=0;
	flags=0;
	from_len = 0;

	sockaddr_in addr = {0};
	sctp_sndrcvinfo sinfo = {0};
	sctp_event_subscribe event = {0};
	sctp_initmsg initmsg = {0};
	pRecvBuffer[RECVBUFSIZE + 1] = (0);
	isServer = h.isServer;

	if (isServer) {
		if ((SctpScocket = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0){
			throw 1;
		}
		if (setsockopt(SctpScocket, IPPROTO_SCTP, SCTP_EVENTS, &event, sizeof(struct sctp_event_subscribe)) < 0){
			throw 2;
		}

		addr.sin_family = AF_INET;
		addr.sin_port = htons(h.port);
		addr.sin_addr.s_addr = inet_addr(h.addr1);


		if (bind(SctpScocket, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) < 0){
			throw 33;
		}
		if (listen(SctpScocket, 1) < 0) {
			throw 4;
		}
	}else{
		if ((SctpScocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
			throw 1;
		}
		initmsg.sinit_num_ostreams = 1;  //number of output streams can be greater
		if (setsockopt( SctpScocket, IPPROTO_SCTP, SCTP_INITMSG, &initmsg,
				sizeof(initmsg))<0 )
		{
			throw 2;
		}

		//other endpoint to make association with
		bzero( (void *)&addr, sizeof(addr) );
		addr.sin_family = AF_INET;
		addr.sin_port = htons(h.port);
		addr.sin_addr.s_addr = inet_addr( h.addr1 );

		//establish SCTP association
		if (connect( SctpScocket, (struct sockaddr *)&addr, sizeof(addr) ) < 0)
		{
			throw 3;
		}
	}

	//wait for connections

}


void CawlSocket::send(Packets::CawlPacket& p) {
	//memcpy(szMsg,&p.getData(),RECVBUFSIZE);
	if (sctp_sendmsg(SctpScocket, (const void *)p.data, strlen(p.data), (struct
			sockaddr *)&addr, from_len, htonl(PPID), 0, 0 /*stream 0*/ , 0, 0) < 0){
		throw 5;
	}
}

void CawlSocket::rec(Packets::CawlPacket& p) {
	while(true)
	{
		//each time erase the stuff
		flags = 0;
		memset((void *)&addr, 0, sizeof(struct sockaddr_in));
		from_len = (socklen_t)sizeof(struct sockaddr_in);
		memset((void *)&sinfo, 0, sizeof(struct sctp_sndrcvinfo));

		n = sctp_recvmsg(SctpScocket, (void*)pRecvBuffer, RECVBUFSIZE,
				(struct sockaddr *)&addr, &from_len, &sinfo, &flags);
		if (-1 == n)
		{
			sleep(1);
			continue;
		}

		if (flags & MSG_NOTIFICATION)
		{
			throw 7;
		}
		else
		{
			p.data = pRecvBuffer;
			break;
		}
	}
}

CawlSocket::~CawlSocket() {
	if (close(SctpScocket) < 0) {
		throw 6;
	}
}

}

