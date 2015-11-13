/*
 * SCTPTest.cpp
 *
 *  Created on: Sep 26, 2014
 *      Author: cawl-server
 */
#define SERV_IP "127.0.0.1"
#define CLIENT_IP "127.0.0.1"
#define S_PORT 5079
#define addr_count 1
#define RECVBUFSIZE	256
#define PPID	1234

//Network
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
//#include <netinet/sctp.h>
#include <arpa/inet.h>
//standard stuff
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <errno.h>
//Our own code
#include "../Netapi/cawl.h" //This is SCTP
#include "../Packets/SimPack.h"
using namespace Packets;
using namespace std;

void client(){
	socklen_t addrSize = sizeof(struct sockaddr_in);
	SimPack clientPack;
	float toggle = 1.0;
	int sctpClient, in, flags;
	char * szMsg;
	int iMsgSize = sizeof(clientPack.fromSim);
	socklen_t opt_len;

	struct sockaddr_in servAddr,clientAddrOne, clientAddrTwo;

	//clientAddresses = malloc((addrSize * addr_count));
	servAddr = {0};
	struct sctp_status status = {0};
	struct sctp_sndrcvinfo sndrcvinfo = {0};
	struct sctp_event_subscribe events = {0};
	struct sctp_initmsg initmsg = {0};

	socklen_t from_len = sizeof(struct sockaddr_in);

	//Create the socket, fails if lksctp doesn't exist
	if ((sctpClient = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP)) < 0) {
		printf("After socket errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	Netapi::cawl cawlClient = Netapi::cawl(sctpClient);

	printf("socket created...\n");
	initmsg.sinit_num_ostreams = 1;


	//Set socket options for client
	if (setsockopt( sctpClient, IPPROTO_SCTP, SCTP_INITMSG, &initmsg,sizeof(initmsg))<0 ){
		printf("After setsockopt errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	printf("setsockopt succeeded...\n");

	//server address  information, zero the address struct
	//bzero( (void *)&servAddr, sizeof(servAddr) );
	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(S_PORT);
	servAddr.sin_addr.s_addr = inet_addr( SERV_IP );

	//establish SCTP association
	if (connect( sctpClient, (struct sockaddr *)&servAddr, sizeof(servAddr) ) < 0){
		printf("After connect errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	printf("connect succeeded...\n");

	opt_len = (socklen_t) sizeof(struct sctp_status);
	if (getsockopt(sctpClient, IPPROTO_SCTP, SCTP_STATUS, &status, &opt_len) < 0){
		printf("After getsockopt errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}else{
		printf("Association ID\t\t= %d\n", status.sstat_assoc_id );
		printf("Receiver window size\t= %d\n", status.sstat_rwnd );
	}
	//send message to server
	int send = 30;
	while(send){
		printf("sending\n");
		send--;
		if(toggle){
			toggle = 0;
			clientPack.setAnalog(1, (toggle*1.0));
		}else{
			toggle = 1;
			clientPack.setAnalog(1, (toggle*1.0));
		}
		char sendbuffer[255];
		memcpy(&sendbuffer,&clientPack.fromSim, sizeof(clientPack.fromSim));
		printf("sending REALLY\n");
		if (cawlClient.sctp_sendmsg(sctpClient, (char*)sendbuffer,sizeof(sendbuffer), NULL, 0,htonl(PPID), 0, 0 /*stream 0*/, 0, 0) < 0){
			printf("After sctp_sendmsg errno: %d\n", errno);
			perror("Description: ");
			exit(1);
		}
		printf("Message sent\n");
	}
	sctp_sendmsg(sctpClient, (const void *)"exit", sizeof("exit"), NULL, 0, htonl(PPID), 0,0/*stream 0*/, 0,0);
	if (close(sctpClient) < 0) {
		printf("After close errno: %d\n", errno);
		perror("Description: ");
	}
	printf("exiting...\n");
	//cleanup
	close(sctpClient);
}

void server(){
	SimPack servPack;

	int sctpServ, n, flags;
	socklen_t from_len;
	//forget about memset(,0,)
	struct sockaddr_in servAddr = {0};
	struct sctp_sndrcvinfo sinfo = {0};
	struct sctp_event_subscribe event = {0};
	char pRecvBuffer[RECVBUFSIZE + 1] = {0};

	char * szMsg;
	int iMsgSize;
	//here we may fail if sctp is not supported
	if ((sctpServ = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP)) < 0){
		printf("After socket errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	printf("socket created...\n");

	Netapi::cawl cawlServ = Netapi::cawl(sctpServ);

	if (setsockopt(sctpServ, IPPROTO_SCTP, SCTP_EVENTS, &event, sizeof(struct sctp_event_subscribe)) < 0){
		printf("After setsockopt errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	printf("setsockopt succeeded...\n");

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(S_PORT);
	servAddr.sin_addr.s_addr = inet_addr(SERV_IP);
	if (bind(sctpServ, (struct sockaddr *)&servAddr, sizeof(struct sockaddr_in)) < 0){
		printf("After bind errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	printf("bind succeeded...\n");
	if (listen(sctpServ, 1) < 0) {
		printf("After listen errno: %d\n", errno);
		perror("Description: ");
		exit(1);
	}
	printf("listen succeeded...\n");
	while(true)
	{
		printf("Receiving\n");
		//each time erase the stuff
		flags = 0;
		memset((void *)&servAddr, 0, sizeof(struct sockaddr_in));
		from_len = (socklen_t)sizeof(struct sockaddr_in);
		memset((void *)&sinfo, 0, sizeof(struct sctp_sndrcvinfo));
		printf("Receiving, really\n");
		n =cawlServ.sctp_recvmsg(sctpServ, (void*)pRecvBuffer, RECVBUFSIZE,(struct sockaddr *)&servAddr, &from_len, &sinfo, &flags);
		printf("Received\n");
		memcpy(&servPack.fromSim, pRecvBuffer, sizeof(servPack.fromSim));
		if (-1 == n)
		{
			printf("Error with sctp_recvmsg: -1... waiting\n");
			printf("errno: %d\n", errno);
			perror("Description: ");
			sleep(1);
			continue;
		}
		printf("Data received: %f, %f", servPack.getAnalog(1), servPack.getAnalog(2));
		if (flags & MSG_NOTIFICATION){
			printf("Notification received!\n");
			printf("From %s:%u\n", inet_ntoa(servAddr.sin_addr), ntohs(servAddr.sin_port));
		}else{
			printf("Received from %s:%u on stream %d, PPID %d. \n",inet_ntoa(servAddr.sin_addr),ntohs(servAddr.sin_port),sinfo.sinfo_stream,ntohl(sinfo.sinfo_ppid));
		}

		//close server when exit is received
		if (0 == strcmp(pRecvBuffer, "exit")){
			break;
		}
	}//while

	printf("exiting...\n");

	if (close(sctpServ) < 0) {
		perror("close");
	}
	exit(1);
}

int main(int argc, char * args[]){
		if ((argc > 1) && ((std::string)args[1] == "server")){
			printf("server mode\n");
			server();
		}
		if((argc > 1) && ((std::string)args[1] == "client")){
			printf("client mode\n");
			client();
		}

}

