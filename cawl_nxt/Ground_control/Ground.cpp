/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: Robin Bond
 */

#include "Ground.h"
using namespace std;
/*	In the constructor objects for the simulator and the CawlSocket are set up together with
 * 	variables used to copy the data into. The analogpacket is copied into a char* with same size
 * 	and that char* is then copied into the CawlPackets data field.
 * 	TODO: Use a state + a compare function to see if sending a new packet is necessary.
 */
Ground::Ground(char* addressOne, char* addressTwo) {
	sp 				=  Packets::SimPack();
	epao 			= Packets::EBUPacketAnalogOut();
	thetemp 		= (char*) malloc(sizeof(epao));
	//state 			= (char*) malloc(sizeof(epao));
	simulator 		= new Simulator::Sim();
	out 			= new Packets::CawlPacket();
	in 				= new Packets::CawlPacket();
	h 				= Netapi::Host(addressOne, 5555, addressTwo, false);
	//tempValue is used for testing purposes only
	tempValue		= 1.0;
	try{
		socketOut 	=  new Netapi::CawlSocket(h);
	}catch(int e){
		throw e;
	}

}
/*	Written by Robin Bond and modified by H�kan Ther�n
 * The sendPacket method receives a packet from the simulator containing data
 * 	on how the current position on controls are. This packet is then translated
 * 	into an EBUPacketAnalogOut with the method setEBUOne, needless to say another
 * 	function for the second ebu is also needed.
 * 	The analogOut Packet is then copied into a char array which will be put into
 * 	the datafield of the cawlpacket that will be sent through the cawlsocket.
 * 	As this will be done with theads a mutex for the cawlsocket is needed.
 */
void Ground::sendPacket() {
	int prio = 1;			// <---- borde sättas beroende av paket
	int streamID = 1;		// <----
	sp = simulator->recPac();

	// Testdata
	/*
	if(tempValue == 1.0){
		tempValue = 0.0;
		sp.fromSim.analog[3] = tempValue;
		sp.fromSim.analog[2] = tempValue;
	}else{
		tempValue = 1.0;
		sp.fromSim.analog[3] = tempValue;
		sp.fromSim.analog[2] = tempValue;
	}
	*/
	setEbuOne(&sp, &epao);
	memcpy(thetemp, &epao, sizeof(epao));
	if(memcmp(thetemp,state,sizeof(epao))){
		out->SetPrio(prio);
		out->SetId(streamID);
		memcpy(&out->data, thetemp, sizeof(epao));
		//memcpy(state, thetemp, sizeof(epao));
		//LÅS MUTEX ETC!!!!
		m_cawlSocket.lock();
		try{
			socketOut->send(*out);
		}catch(int e){
			throw e;
		}
		m_cawlSocket.unlock();
	}
}

/* Written by H�kan Ther�n
 * Does nothing with the data atm.
 */
void Ground::receivePacket(){
	m_cawlSocket.lock();
	try{
		socketOut->rec(*out);
	}catch(int e){
		throw e;
	}
	m_cawlSocket.unlock();

	// return the data???
}

int Ground::setBoom(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value  * 2 + 2.5;
	pkt->setChannelValue(5-temp, AO_9);
	pkt->setChannelValue(temp, AO_10);
	return 1;
}

int Ground::setBucket(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value * 2 + 2.5;
	pkt->setChannelValue(5-temp, AO_11);
	pkt->setChannelValue(temp, AO_12);
	return 1;
}

void Ground::setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao) {
	epao->setDestination(1);
	Packets::commandPacket simData = sp->getData();
	setBoom((float)simData.analog[2], epao);
	setBucket((float)simData.analog[3], epao);
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
	delete out;
	delete in;
}
