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
	slen = sizeof(grAddr);
	sp 				=  SimPack();
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
/*	Written by Robin Bond and modified by Håkan
 * The sendPacket method receives a packet from the simulator containing data
 * 	on how the current position on controls are. This packet is then transferred to Mobile.
 * 	the state must be fixed and a separate thread must be created for receiving packets and changing the state
 */
void Ground::sendPacket() {
	try{
		//unique_lock<mutex> lock1(m_state, defer_lock);
		SimPack temp;
		m_state.lock();
		temp = state;
		m_state.unlock();
		sendto(grSocket, (char*)&temp.fromSim, sizeof(temp.fromSim), 0, (struct sockaddr*) &grAddr, slen);

	}catch(int e){
		throw e;
	}
}
void Ground::receiveSimPack(){
	sp = simulator->recPac();
	m_state.lock();
	if(not (sp == state)){
		state = sp;
	}
	m_state.unlock();

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


/* Written by H�kan Ther�n
 * Used in the threads that will run
 */
void Ground::startRecieve(){
	while(true){
		receiveSimPack();
	}
}
/* Written by H�kan Ther�n
 * Used in the threads that will run
 */
void Ground::startSend(){
	while(true){
		sendPacket();
	}
}

Ground::~Ground() {
	delete simulator;
}
