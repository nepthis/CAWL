/*
 * Mobile.cpp v0.2
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */

#include "Mobile.h"

namespace Major_Tom {

Mobile::Mobile() {
	//Variable setup
	ready = false;
	pleased = false;
	stopPacket 			= Packets::EBUPacketAnalogOut();
	rPack						= Packets::EBURelayPacket();
	h									= Netapi::Host((char*)"127.0.0.1", 5555, (char*)"127.0.0.1", true);
	q_cawlBuffer 		= std::queue<Packets::CawlPacket>();
	em 							= EBU::EBUManager();
	//Conflicts will occuf if it's possible to insert
	//and remove elements at the same time
	std::mutex m_Queue;
	//For the Socket in order to be able to send and receive
	std::mutex m_Cawl;
	std::condition_variable cond;
	//-------------------------------------------------------------------------
	rPack.setRelayValue(R_A9,1);  		//9 and 10 are used for
	rPack.setRelayValue(R_A10,1);	// boom
	rPack.setRelayValue(R_A11,1);	//11 and 12 are used for bucket
	rPack.setRelayValue(R_A12,1);
	ebuMan.sendRelayCommand(rPack, 1);
}
void Mobile::startUp(bool condition){
	while(condition){
			gatewaySocket	= Netapi::CawlSocket(h);
			condition = false;
		sleep(1); //timeout for retrying
	}
}
void Mobile::socketReceive() {
	Packets::CawlPacket recPack = Packets::CawlPacket();
	while(not pleased){
		std::unique_lock<std::mutex> lck (m_Cawl);

	}
}

void Mobile::socketSend() {
}

void Mobile::ebuSend() {
}

Mobile::~Mobile() {
	ebuMan.sendAnalogCommand(stopPacket, 1);
	resetRelays();
}


} //namespace Major_Tom

