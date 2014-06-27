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

CawlSocket::CawlSocket() {
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
	isServer = false;

	//server addr params
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(1);
	saddr.sin_addr.s_addr = inet_addr("0.0.0.0");
	SctpScocket = 0;

	gm = GatherMetrics();
	gm.setOption("DELAY", true);
	gm.setOption("CHKSUMERR",false);

}


CawlSocket::CawlSocket(Netapi::Host& h) {
	n=0;
	flags=0;
	from_len = 0;
	metrics = false;

	gm = GatherMetrics();

	//Options can be set "on the go" as well.
	gm.setOption("DELAY", true);
	gm.setOption("CHKSUMERR",true);


	sockaddr_in addr = {0};
	sockaddr_in saddr = {0};
	sctp_sndrcvinfo sinfo = {0};
	sctp_event_subscribe event = {0};
	sctp_initmsg initmsg = {0};
	pRecvBuffer[RECVBUFSIZE + 1] = (0);
	isServer = h.isServer;

	//server addr params
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(h.GetPort());
	saddr.sin_addr.s_addr = inet_addr(h.addr2);

	//other endpoint addr info
	bzero( (void *)&addr, sizeof(addr) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons(h.GetPort());
	addr.sin_addr.s_addr = inet_addr( h.addr1 );

	// Number of streams
	initmsg.sinit_num_ostreams = 1;
	initmsg.sinit_max_instreams = 1;

	// Depending on the socket being server or not SCTP is initialized different
	// First setup socket, if succeded setup SCTP-options for socket then bind to addr
	// and listen or try connect to server depending on isServer true/false
	// TODO
	// Add nr of streams and handling

	if (isServer) {
		//create a sctp socket
		if ((SctpScocket = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0){
			throw 1;
		}
		//specific sctp socket options is set with setsockopt. tells socket to use sctp_event_subscribe
		if (setsockopt(SctpScocket, IPPROTO_SCTP, SCTP_EVENTS, &event, sizeof(struct sctp_event_subscribe)) < 0){
			throw 2;
		}
		// bind the socket to specific interface should use bindx in future to use multiple interfaces
		if (bind(SctpScocket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0){
			throw 3;
		}
		// listen for connecting sockets from client side
		if (listen(SctpScocket, 1) < 0) {
			throw 4;
		}
	}else{
		//create a sctp socket
		if ((SctpScocket = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
			throw 5;
		}
		// Used to setup SCTP_NODELAY, tells Sctp socket to not use Nagles algorithm (produces delay)
		long value = 1;
		if ((setsockopt( SctpScocket, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg))||
			 setsockopt( SctpScocket, IPPROTO_SCTP, SCTP_NODELAY, &value,sizeof(value)))<0 )
		{
			throw 6;
		}

		//try to connect to listening socket on server side
		if (connect( SctpScocket, (struct sockaddr *)&addr, sizeof(addr) ) < 0)
		{
			errno = EHOSTDOWN;
			throw 7;
		}
	}
}


// Send data from CawlPacket using established socket
// TODO add stream param etc.

void CawlSocket::send(Packets::CawlPacket& p) {
	// sets timestamp on packet for delay measurement. Should instead use RTT measurement, NTP is not accurate enough
	p.SetSnd();
	if (isServer){
		if (sctp_sendmsg(SctpScocket, (char*)&p, sizeof(p), (struct
				sockaddr *)&addr, from_len, htonl(PPID), 0, 0 /*stream 0*/ , 0, 0) < 0){
			throw 8;
		}
	}else{
		if (sctp_sendmsg(SctpScocket, (char*)&p, sizeof(p), NULL, 0,
				htonl(PPID), 0, 0 /*stream 0*/, 0, 0) < 0)
		{
			throw 9;
		}
	}
	/* FUTURE WORK:
	 *  Add measuring here as well when options relevant to sending
	 * exists.
	 */
}
//Recieve data from endpoint and store data in Cawlpacket

void CawlSocket::rec(Packets::CawlPacket& p) {
	while(true)
	{
		// erase the placeholders
		flags = 0;
		memset((void *)&addr, 0, sizeof(struct sockaddr_in));
		from_len = (socklen_t)sizeof(struct sockaddr_in);
		memset((void *)&sinfo, 0, sizeof(struct sctp_sndrcvinfo));
		memset((void *)&pRecvBuffer, 0,RECVBUFSIZE);

		n = sctp_recvmsg(SctpScocket, (void*)pRecvBuffer, RECVBUFSIZE,
				(struct sockaddr *)&addr, &from_len, &sinfo, &flags);
		if (-1 == n) {
			continue;
		}
		if (flags & MSG_NOTIFICATION){
			throw 10;

		} else {
			Packets::CawlPacket packet;
			memcpy(&packet,&pRecvBuffer, sizeof(Packets::CawlPacket));
			packet.SetRcv(); //set arrival time of the packet
			p = packet;
			if(metrics){
				try{
					gm.measureDelay(p, 1, "testing");
					//gm.measuredata(p, 1, "test"); //change the "test" into the use of a variable that can be set with a function
				}catch(int e){
					throw e;
				}
			}
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
