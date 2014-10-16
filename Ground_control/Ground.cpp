/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */

#include "Ground.h"
using namespace std;
using namespace Packets;
mutex m_state;

Ground::Ground() {
	ss = serSend();
	slen = sizeof(grAddr);
	sp 				=  SimPack();
	simulator 		= new Simulator::Sim();
	if ((grSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		throw grSocket; //TBD
	}
	memset((char *)&grAddr, 0, sizeof(grAddr));
	inet_pton(AF_INET, DEST_ADDR, &(grAddr.sin_addr));
	grAddr.sin_port = htons(DEST_PORT);
}
/*	Written by Robin Bond and modified by Håkan
 * The sendPacket method receives a packet from the simulator containing data
 * 	on how the current position on controls are. This packet is then transferred to Mobile.
 * 	the state must be fixed and a separate thread must be created for receiving packets and changing the state
 */
void Ground::sendMobile() {
	while(true){
		usleep(1000);
		SimPack temp;
		m_state.lock();
		temp = state;
		m_state.unlock();
		sendto(grSocket, (char*)&temp.fs, sizeof(temp.fs), 0, (struct sockaddr*) &grAddr, slen);
	}
}
void Ground::receiveSim(){
	while(true){
		sp = simulator->recvSim();
		if(sp.getAnalog(BRAKEPEDAL) > abs(0.5)){ss.sndPulse();}
		m_state.lock();
		if(not (sp == state)){state = sp;}
		m_state.unlock();
	}
}

/* This function also needs a state for the IMU data and then anoher function can call sim and send
 * 	the state
 */
void Ground::receiveImuPacket(){
	char buffer[255];
	try{
		ImuPack imp = ImuPack();
		recvfrom(recImuSocket, buffer, 255, 0, (struct sockaddr *)&recImuAddr, &slen);
		memcpy(&imp.sens, buffer, sizeof(imp.sens));
	}catch(int e){
		throw e;
	}
}


Ground::~Ground() {
	delete simulator;
}
