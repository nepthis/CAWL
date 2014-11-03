/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */

#include "Ground.h"
using namespace std;
using namespace Packets;
using namespace Ground_control;
mutex m_state;

Ground::Ground(bool sctpStatus) {
	sctpIsOn = sctpStatus;
	slen = sizeof(grAddr);
	sp 				=  SimPack();
	simulator 		= new Sim();
	if(sctpIsOn){

	}else{
		//-------------------------------------UDP------------------------------------------
		if ((grSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
			perror("socket error");
			exit(0);}
		memset((char *)&grAddr, 0, sizeof(grAddr));
		inet_pton(AF_INET, DEST_ADDR, &(grAddr.sin_addr));
		grAddr.sin_port = htons(DEST_PORT);
		//----------------------------------------------------------------------------------
	}

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
		if(sctpIsOn){

		}else{
			if(sendto(grSocket, (char*)&temp.fs, sizeof(temp.fs), 0, (struct sockaddr*) &grAddr, slen) < 0){
				perror("Ground:sendMobile");
				exit(0);
			}
		}

	}
}
void Ground::receiveSim(){
	while(true){
		sp = simulator->recvSim();
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
