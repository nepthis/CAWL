/*
 * Mobile.cpp v0.2
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */

#include "Mobile.h"

namespace Major_Tom {

Mobile::Mobile(char* addressOne, char* addressTwo) {
	//Variable setup
	try{
		pleased = false;
		stopPacket 			= Packets::EBUPacketAnalogOut();
		rPack						= Packets::EBURelayPacket();
		//h1								= Netapi::Host((char*)addressOne, 5555, (char*)addressTwo, false);
		h2								= Netapi::Host((char*)"127.0.0.2", 5555, (char*)"127.0.0.1", true);
		q_cawlBuffer 		= std::queue<Packets::EBUPacketAnalogOut>();
		em 							= EBU::EBUManager();
		rPack.setRelayValue(R_A9,1);  		//9 and 10 are used for
		rPack.setRelayValue(R_A10,1);	// boom
		rPack.setRelayValue(R_A11,1);	//11 and 12 are used for bucket
		rPack.setRelayValue(R_A12,1);
		em.sendRelayCommand(rPack, 1);
	}catch(int e){
		perror("Description");
		throw e;
	}

	//-------------------------------------------------------------------------

}
void Mobile::startUp(){
	//gatewaySocketSend	= Netapi::CawlSocket(h1);
	try{
		gatewaySocketReceive = Netapi::CawlSocket(h2);
	}catch(int e){
		perror("Desc");
	}

}
void Mobile::socketReceive() {
	Packets::CawlPacket recPack = Packets::CawlPacket();
	while(not pleased){
		//std::unique_lock<std::mutex> lock1(m_Cawl, std::defer_lock);
		std::unique_lock<std::mutex> lock2(m_Queue, std::defer_lock);
		Packets::EBUPacketAnalogOut analogOut =  Packets::EBUPacketAnalogOut();
		//std::lock(lock1, lock2);
		lock2.lock();
		try{
			gatewaySocketReceive.rec(recPack);
			char *tempbuff;
			tempbuff = (char*) malloc(sizeof(analogOut));
			memcpy(tempbuff, recPack.data, sizeof(analogOut));
			memcpy(&analogOut, tempbuff, sizeof(analogOut));
			q_cawlBuffer.push(analogOut);
		}catch(int e){
			errno = 1; //Number TBD
		}

	}
}

void Mobile::socketSend() {
	while(not pleased){
		sleep(1);
		std::unique_lock<std::mutex> lock1(m_Cawl, std::defer_lock);
		lock1.lock();
		Packets::EBUPacketAnalogOut sendBackPacket = Packets::EBUPacketAnalogOut();
		Packets::CawlPacket *ut = new Packets::CawlPacket(0, 0);
		try{
			sendBackPacket.setChannelValue(5, AO_9);
			sendBackPacket.setChannelValue(5, AO_10);
			memcpy(&ut->data, &sendBackPacket ,sizeof(sendBackPacket));
			gatewaySocketSend.send(*ut);
		}catch(int e){
			errno = ECOMM;
			//throw 0;
		}
	}

}

void Mobile::ebuSend() {
	while(not pleased){
		usleep(200000); 		//The usleep prevents the EBU from crashing...
		std::unique_lock<std::mutex> lock1(m_Queue, std::defer_lock);
		lock1.lock();
		Packets::EBUPacketAnalogOut sendToEBU;
		try{
			sendToEBU = q_cawlBuffer.front();
			q_cawlBuffer.pop();
			em.sendAnalogCommand(sendToEBU,sendToEBU.getDestination());
		}catch(int e){
			errno = ECOMM;
			//throw 0;
		}
	}
}

Mobile::~Mobile() {
	em.sendAnalogCommand(stopPacket, 1);
	rPack = Packets::EBURelayPacket();
	em.sendRelayCommand(rPack, 1);
}


} //namespace Major_Tom

