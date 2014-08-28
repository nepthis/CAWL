/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */

#include "Ground.h"
using namespace std;
//	In the constructor, objects for the simulator and UDP socket are set up.
Ground::Ground() {
	slen = sizeof(grAddr);
	sp 				=  Packets::SimPack();
	thetemp 		= (char*) malloc(sizeof(sp.fromSim));
	simulator 		= new Simulator::Sim();
	if ((grSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw grSocket; //TBD
	}
	memset((char *)&grAddr, 0, sizeof(grAddr));
	inet_pton(AF_INET, SND_ADDR, &(grAddr.sin_addr));
	grAddr.sin_port = htons(SND_PORT);
}

/*	Written by Robin Bond and modified by H�kan Ther�n
 * The sendPacket method receives a packet from the simulator containing data
 * 	on how the current position on controls are. This packet is then transferred to Mobile.
 */
void Ground::sendPacket() {
	sp = simulator->recPac();
	printf("Packet received from sim\n");
	try{
		if (not (sp == state)){
			state = sp;
			sendto(grSocket, (char*)&state.fromSim, sizeof(state.fromSim), 0, (struct sockaddr*) &grAddr, slen);
			printf("Packet sent to mobile\n");
		}
	}catch(int e){
		throw e;
	}
}

/* Written by H�kan Ther�n
 * Does nothing with the data atm.
 */
void Ground::receivePacket(){
	try{
	}catch(int e){
		throw e;
	}
}


/* Written by H�kan Ther�n
 * Used in the threads that will run
 */
void Ground::startRecieve(){
	while(true){
		receivePacket();
	}
}
/* Written by H�kan Ther�n
 * Used in the threads that will run
 */
void Ground::startSend(){
	while(true){
		sendPacket();
		usleep(5000);
	}
}

Ground::~Ground() {
	delete simulator;
}
