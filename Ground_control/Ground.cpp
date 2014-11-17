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
	//	if(sctpIsOn){
	//------------------------------------SCTP------------------------------------------
	//		sockaddr_in addrSCTP = {0};
	//		sctp_sndrcvinfo sinfoSCTP = {0};
	//		sctp_event_subscribe eventSCTP = {0};
	//		pRecvBuffer[RECVBUFSIZE + 1] = {0};
	//		//MORE

	//}else{
	//-------------------------------------UDP------------------------------------------
	if ((grSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		logError(strerror(errno));
		exit(1);}
	memset((char *)&grAddr, 0, sizeof(grAddr));
	inet_pton(AF_INET, DEST_ADDR, &(grAddr.sin_addr));
	grAddr.sin_port = htons(DEST_PORT);
	logVerbose("UDP socket for remote commands is set up");
	//----------------------------------------------------------------------------------
	//	}

}
/*	Written by Robin Bond and modified by Håkan
 * The sendPacket method receives a packet from the simulator containing data
 * 	on how the current position on controls are. This packet is then transferred to Mobile.
 * 	the state must be fixed and a separate thread must be created for receiving packets and changing the state
 */
void Ground::sendMobile() {
	int errorMobile = 0;
	logVerbose("Ground -> sendMobile: starting");
	while(true){
		usleep(1000);
		SimPack temp;
		m_state.lock();
		temp = state;
		m_state.unlock();
		//if(sctpIsOn){

		//	}else{
		if(sendto(grSocket, (char*)&temp.fs, sizeof(temp.fs), 0, (struct sockaddr*) &grAddr, slen) < 0){
			logWarning("Ground:sendMobile: ");
			logWarning(strerror(errno));
			errorMobile++;
			continue;
		}else{
			errorMobile = 0;
		}
		if(errorMobile == 1000){
			errno = ECOMM;
			logError(strerror(errno));
			logError("Fatal: cannot send to Mobile: ");
			exit(1);
		}
		//}

	}
}
void Ground::receiveSim(){
	int errorsSim = 0;
	logVerbose("Ground -> receiveSim: starting");
	while(errorsSim <= 50){
		try{
			sp = simulator->recvSim();
			errorsSim = 0;
		}
		catch(int e){
			logWarning("Ground -> receiveSim: could not receive packet from the simulator");
			errorsSim ++;
			continue;
		}
		m_state.lock();
		if(not (sp == state)){state = sp;}
		m_state.unlock();
		continue;
	}
	errno = ENETUNREACH;
	logError("Fatal: Ground -> receiveSim");
	logError(strerror(errno));
	exit(1);
}

/* This function also needs a state for the IMU data and then another function can call sim and send
 * 	the state
 */
void Ground::receiveImuPacket(){
	char buffer[255];
	Packets::ImuPack impa = Packets::ImuPack();
	logVerbose("Ground -> receiveImuPacket: starting");
	IMU::IMUManager im = IMU::IMUManager();
	im.init(false, true);
	while(true){
		try{
			if(recvfrom(recImuSocket, buffer, 255, 0, (struct sockaddr *)&recImuAddr, &slen) <0 ){
				logWarning("Ground -> receiveImuPacket: could not receive imuPacket");
				//im.setImuPack(Packets::ImuPack());
				continue;
			}
			if(memcpy(&impa.sens, buffer, sizeof(impa.sens)) <0){
				logError(strerror(errno));
				logError("Fatal: Ground -> receiveImuPacket: memcpy");
			}
			im.setImuPack(impa);
		}catch(int e){
			logError(strerror(errno));
			logError("Fatal: Ground -> receiveImuPacket");
			delete &im;
			sleep(5);
			exit(1);
		}
	}
}



Ground::~Ground() {
	delete simulator;
}


