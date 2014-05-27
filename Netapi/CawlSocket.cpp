/*
 * CawlSocket.cpp
 *
 *  Created on: May 7, 2014
 *      Author: Håkan Therén
 *
 *      Thanks to alexkr.com for examples of SCTP implementaion
 *
 *      Description: Class to use within CAWL-framework for sending and recieving data
 *      			 via SCTP in an easy to use manner.
 *
 */

#include "CawlSocket.h"


namespace Netapi {

CawlSocket::CawlSocket(Netapi::Host& h) {
	n=0;
	flags=0;
	from_len = 0;
	metrics = false;

	sockaddr_in addr = {0};
	sockaddr_in saddr = {0};
	sctp_sndrcvinfo sinfo = {0};
	sctp_event_subscribe event = {0};
	sctp_initmsg initmsg = {0};
	pRecvBuffer[RECVBUFSIZE + 1] = (0);
	isServer = h.isServer;

	//server addr params
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(h.port);
	saddr.sin_addr.s_addr = inet_addr(h.addr2);

	//other endpoint addr info
	bzero( (void *)&addr, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons(h.port);
	addr.sin_addr.s_addr = inet_addr( h.addr1 );

	// Number of streams
	initmsg.sinit_num_ostreams = 1;

	// Depending on the socket being server or not SCTP is initialized different
	// First setup socket, if succeded setup SCTP-options for socket then bind to addr
	// and listen or try connect to server depending on isServer true/false
	// TODO
	// Add nr of streams and handling

	if (isServer) {
		if ((SctpScocket = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0){
			throw 1;
		}
		if (setsockopt(SctpScocket, IPPROTO_SCTP, SCTP_EVENTS, &event, sizeof(struct sctp_event_subscribe)) < 0){
			throw 1;
		}
		if (bind(SctpScocket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0){
			throw 2;
		}
		if (listen(SctpScocket, 1) < 0) {
			throw 2;
		}
	}else{
		if ((SctpScocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
			throw 1;
		}
		if (setsockopt( SctpScocket, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg))<0 )
		{
			throw 1;
		}

		if (connect( SctpScocket, (struct sockaddr *)&addr, sizeof(addr) ) < 0)
		{
			throw 3;
		}
	}
}


// Send data from CawlPacket using established socket
// TODO add stream param etc.

void CawlSocket::send(Packets::CawlPacket& p) {
	p.SetSnd();
	if (sctp_sendmsg(SctpScocket, /*(const void *)p.data*/(char*)&p, sizeof(p), (struct
			sockaddr *)&addr, from_len, htonl(PPID), 0, 0 /*stream 0*/ , 0, 0) < 0){
		throw 4;
	}
}

//Recieve data from endpoint and store data in Cawlpacket

void CawlSocket::rec(Packets::CawlPacket& p) {
	while(true)
	{
		//each time erase the stuff
		flags = 0;
		memset((void *)&addr, 0, sizeof(struct sockaddr_in));
		from_len = (socklen_t)sizeof(struct sockaddr_in);
		memset((void *)&sinfo, 0, sizeof(struct sctp_sndrcvinfo));
		memset((void *)&pRecvBuffer, 0,RECVBUFSIZE);

		n = sctp_recvmsg(SctpScocket, (void*)pRecvBuffer, RECVBUFSIZE,
				(struct sockaddr *)&addr, &from_len, &sinfo, &flags);
		if (-1 == n) {
			sleep(1);
			continue;
		}
		if (flags & MSG_NOTIFICATION){
			throw 5;

		} else {
			Packets::CawlPacket packet = Packets::CawlPacket((uint8_t)1,(uint8_t)1,(char*)"TESTAAAAAAAAAAAAAAAAAAAAAAAAAAATTTT");
			printf("%i \n",sizeof(packet));
			memcpy(&packet,&pRecvBuffer, sizeof(Packets::CawlPacket));
			packet.SetRcv();
			p = packet;
			break;
		}
	}
}

CawlSocket::~CawlSocket() {
	if (close(SctpScocket) < 0) {
		// Cleanup to be implemented
	}
}
}

