/*
 * Mobile.cpp v0.2
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */
#include "Mobile.h"
using namespace  Major_Tom;
using namespace Packets;
using namespace std;
mutex m_Queue;
mutex m_Sendstate;
/*	The contructor for the Mobile gateway initializes almost everything
 * 	and also sends the relay information to the EBU enabling the needed relays.
 * 	For now sending data back over the CawlSocket is not performed but when it
 * 	is a separate socket for sending the data will be used.
 */
Mobile::Mobile() {
	et = EBU::EBUTranslator();
	//--------------------------------------------- Receiving socket from Ground-------------------------------------------------------
	slen = sizeof(mobAddr);
	if ((mobSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){throw 13;}
	memset((char *)&mobAddr, 0, slen);
	if(inet_pton(AF_INET, REC_ADDR, &(mobAddr.sin_addr)) < 0){throw 13;}
	mobAddr.sin_port = htons(REC_PORT);
	if (bind(mobSocket, (struct sockaddr *)&mobAddr, sizeof(mobAddr)) < 0) {throw 13;}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------Socket for sending IMU data-----------------------------------------------------------
	//	if ((sndImuSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
	//		perror("socket error");
	//		printf ("Error number is: %s\n",strerror(errno));
	//		throw sndImuSocket; //TBD
	//	}
	//	memset((char *)&sndImuAddr, 0, sizeof(sndImuAddr));
	//	inet_pton(AF_INET, GND_ADDR, &(sndImuAddr.sin_addr));
	//	sndImuAddr.sin_port = htons(IMU_PORT);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	pleased = false;
	//		rPackOne.setRelayValue(R_D9,1); 			//ActivationCLC, might need to go to cdc instead...
	rPackOne.setRelayValue(R_S7, 1);		//Relay for brakelights
	rPackOne.setRelayValue(R_S4, 1);		//Relay for parking brake
	//rPackOne.setRelayValue(R_S5, 1);	//Relay for Horn, not workin...maybe.
	rPackTwo.setRelayValue(R_A9,1);  		//Lift/sink 1st
	rPackTwo.setRelayValue(R_A10,1);		// lift/sink 2nd
	rPackTwo.setRelayValue(R_A11,1);		//tilt 1st
	rPackTwo.setRelayValue(R_A12,1);		//tilt 2nd
	rPackTwo.setRelayValue(R_A13, 1);		//Third Function 1st
	rPackTwo.setRelayValue(R_A14, 1);		//Third Function 2nd
	rPackTwo.setRelayValue(R_A15, 1);		//Fourth Function 2nd
	rPackTwo.setRelayValue(R_A16, 1);		//Fourth Function 1st
	rPackTwo.setRelayValue(R_A17, 1);		//CDC Steering
	rPackTwo.setRelayValue(R_A18, 1);		//CDC Steering
	rPackTwo.setRelayValue(R_A19, 1);		//Gas
	rPackTwo.setRelayValue(R_A20, 1);		//Gas
	rPackTwo.setRelayValue(R_A7, 1);		//broms
	rPackTwo.setRelayValue(R_D22,1);		//Gear_Reverse
	rPackTwo.setRelayValue(R_D31,1);		//Gear_Forward
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
}

//This function receives UDP packets and puts them in a buffer
void Mobile::socketReceive() {
	while(not pleased){
		SimPack simpack;
		char recbuf[255];
		try{
			recvfrom(mobSocket, recbuf, 255, 0, (struct sockaddr *)&mobAddr, &slen);
			memcpy(&simpack.fromSim, recbuf, sizeof(simpack.fromSim));
		}catch(int e){
			perror("socketReceive error");
			throw e;
		}
		m_Queue.lock();
		if ((not (state == simpack)) && (state.fromSim.timeStamp < simpack.fromSim.timeStamp)){
			state = simpack;
		}
		m_Queue.unlock();
	}
}
/*	SocketSend, not in use for now, will send data back over to the Ground Gateway.
 * 	The data being sent back is not really important right now and will just be the standard AnalogIn data.
 */
void Mobile::socketSend() {
	while(not pleased){
		try{
			//em.recAnalogIn(1);
			//em.recAnalogIn(2);
		}catch(int e){
			throw e;
		}
	}
}
/*	Receives data from an IMUHandler and puts it into a state.
 * 	Written by Robin
 */
void Mobile::imuRec() {
	//TODO: Use function from håkans IMUHandler and receive IMUdata
}
/*	The method ebuSend locks the packetBuffer and takes out One packet, sends it to the ebu with
 * 	the ebuManager. This method is designed to be started as a thread.
 * 	For now there is a sleep of 200000 microseconds in order to not crash the EBU.
 * 	The data it sends comes from a state which is set in socketReceive() if it is different than the existing one
 */
void Mobile::ebuOneSend() {
	AnalogOut analogOne;
	DigitalOut digitalOne;
	AnalogOut analogTwo;
	DigitalOut digitalTwo;	//Not really used for now
	DigitalIn digitaldummy;
	AnalogIn analogdummy;
	while(not pleased){
		SimPack tempState; //Locking over methods in other objects might cause problem, this is safer.
		m_Queue.lock();
		tempState = state;
		m_Queue.unlock();
		et.setEbuOne(&tempState, &analogOne, &digitalOne);	//Use EBUTranslator (et) to translate simdata
		digitaldummy = em.recDigitalIn();
		analogdummy = em.recAnalogIn();
		if((digitaldummy.getSource() == 1) && analogdummy.getSource()==1){
			em.sendAnalogCommand(analogOne.getChannel(), analogOne.getDestination());
			em.sendDigitalCommand(digitalOne.getChannel(), digitalOne.getDestination());
			continue;
		}
		if((digitaldummy.getSource() == 2) && analogdummy.getSource()==2){
			em.sendAnalogCommand(analogTwo.getChannel(), analogTwo.getDestination());
			em.sendDigitalCommand(digitalTwo.getChannel(), digitalTwo.getDestination());
			continue;
		}
	}
}
void Mobile::ebuTwoSend() {
	AnalogOut analogTwo;
	DigitalOut digitalTwo;	//Not really used for now
	DigitalIn digitaldummy;
	AnalogIn analogdummy;
	while(not pleased){
		SimPack tempState; //Locking over methods in other objects might cause problem, this is safer.
		m_Queue.lock();
		tempState = state;
		m_Queue.unlock();
		et.setEbuTwo(&tempState, &analogTwo, &digitalTwo);
		digitaldummy = em.recDigitalIn();
		analogdummy = em.recAnalogIn();
		if((digitaldummy.getSource() == 2) && analogdummy.getSource()==2){
			em.sendAnalogCommand(analogTwo.getChannel(), analogTwo.getDestination());
			em.sendDigitalCommand(digitalTwo.getChannel(), digitalTwo.getDestination());
		}
	}
}
//void Mobile::sendEbuOne(){
//
//}


Mobile::~Mobile() {
	em.sendAnalogCommand(stopPacket.getChannel(), 1);
	rPackOne = RelayOut();
	rPackTwo = RelayOut();
	em.sendRelayCommand(rPackOne, 1);
	em.sendRelayCommand(rPackTwo, 2);
}



