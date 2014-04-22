/*
 * Server.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-mobile
 */

#include "Server.h"

Server::Server(Host c) {
	openlog("Cawl", 0, LOG_USER);
	int SctpSocket;

    struct sockaddr_in addr = {0};
    struct sctp_sndrcvinfo sinfo = {0};
    struct sctp_event_subscribe event = {0};
    char pRecvBuffer[RECVBUFSIZE + 1] = {0};

	if ((SctpSocket = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0)
	       {
					syslog(LOG_INFO, "After socket errno: %d\n", errno);
					exit(0);
	       }


}

Server::~Server() {
	closelog();
}

