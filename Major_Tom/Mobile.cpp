/*
 * Mobile.cpp v0.2
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */

#include "Mobile.h"

namespace Major_Tom {
/*	The contructor for the Mobile gateway initializes almost everything
 * 	and also sends the relay information to the EBU enabling the needed relays.
 * 	For now sending data back over the CawlSocket is not performed but when it
 * 	is a separate socket for sending the data will be used.
 */
Mobile::Mobile(char* addressOne, char* addressTwo) {
	//Variable setup
	pleased = false;
	gatewaySocketReceive 	= NULL;
	gatewaySocketSend 		= NULL;
	try{
		stopPacket 				= Packets::EBUPacketAnalogOut();
		rPack					= Packets::EBURelayPacket();
		//h1					= Netapi::Host((char*)addressOne, 5555, (char*)addressTwo, false);
		h2						= Netapi::Host((char*)"127.0.0.2", 5555, (char*)"127.0.0.1", true);
		q_cawlBuffer 			= std::queue<Packets::EBUPacketAnalogOut>();
		em 						= EBU::EBUManager();
		rPack.setRelayValue(R_A9,1);  		//9 and 10 are used for
		rPack.setRelayValue(R_A10,1);	// boom
		rPack.setRelayValue(R_A11,1);	//11 and 12 are used for bucket
		rPack.setRelayValue(R_A12,1);
		em.sendRelayCommand(rPack, 1);
	}catch(int e){
		throw e;
	}

	//-------------------------------------------------------------------------

}
/*This method is used to initialize the CawlSockets, for now only one receicing socket is used.
 *
 */
void Mobile::startUp(){
	//gatewaySocketSend	= Netapi::CawlSocket(h1);
	try{
		gatewaySocketReceive = new Netapi::CawlSocket(h2);
		gatewaySocketReceive->setmetrics(true);
	}catch(int e){
		throw e;
	}

}
/*	The method socketReceive is used to receive packets from the CawlSocket and then
 * 	take the data field from the cawlpacket and put it into an EBUPacketAnalogOut
 * 	which will be pushed into a packetbuffer. As this method will be started as a thread
 * 	mutexes for shared variables are needed, for now there is only one used for the
 * 	packetbuffer.
 *
 */
void Mobile::socketReceive() {
	Packets::CawlPacket recPack = Packets::CawlPacket();
	while(not pleased){
		std::unique_lock<std::mutex> lockQ(m_Queue, std::defer_lock);
		Packets::EBUPacketAnalogOut analogOut =  Packets::EBUPacketAnalogOut();
		lockQ.lock();
		try{
			gatewaySocketReceive->rec(recPack);
			char *tempbuff;
			tempbuff = (char*) malloc(sizeof(analogOut));
			memcpy(tempbuff, recPack.data, sizeof(analogOut));
			memcpy(&analogOut, tempbuff, sizeof(analogOut));
			q_cawlBuffer.push(analogOut);
		}catch(int e){
			throw e;
		}

	}
}
/*	SocketSend, not in use for now, will send data back over the CawlSocket to the Ground Gateway.
 * 	Data that should be sent back could be data for the video stream or audio feedback etc
 */
void Mobile::socketSend() {
	while(not pleased){
		Packets::EBUPacketAnalogOut sendBackPacket = Packets::EBUPacketAnalogOut();
		Packets::CawlPacket *ut = new Packets::CawlPacket(0, 0);
		try{
			sendBackPacket.setChannelValue(5, AO_9);
			sendBackPacket.setChannelValue(5, AO_10);
			memcpy(&ut->data, &sendBackPacket ,sizeof(sendBackPacket));
			gatewaySocketSend->send(*ut);
		}catch(int e){
			throw e;
		}
	}

}
/*	The method ebuSend locks the packetBuffer and takes out One packet, sends it to the ebu with
 * 	the ebuManager. This method is designed to be started as a thread.
 * 	For now there is a sleep of 200000 microseconds in order to not crash the EBU.
 */
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
			throw 0;
		}
	}
}

Mobile::~Mobile() {
	em.sendAnalogCommand(stopPacket, 1);
	rPack = Packets::EBURelayPacket();
	em.sendRelayCommand(rPack, 1);
	delete gatewaySocketSend;
	delete gatewaySocketReceive;
}

} //namespace Major_Tom
