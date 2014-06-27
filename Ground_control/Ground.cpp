/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: cawl-server
 */

#include "Ground.h"
using namespace std;

Ground::Ground(char* addressOne, char* addressTwo) {
	countBoomUp = countBuckUp = countBoomDown = countBuckDown = 0;
	sp 				=  Packets::SimPack();
	epao 			= Packets::EBUPacketAnalogOut();
	thetemp 		= (char*) malloc(sizeof(epao));
	state 			= (char*) malloc(sizeof(epao));
	simulator 		= new Simulator::Sim();
	out 			= new Packets::CawlPacket();
	in 				= new Packets::CawlPacket();
	h 				= Netapi::Host(addressOne, 5555, addressTwo, false);
	tempValue		= 1.0;
	try{
		socketOut 	=  new Netapi::CawlSocket(h);
	}catch(int e){
		throw e;
	}

}

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
	//printf("Values from the sim in the analog packet: %i, %i\n", epao.getChannelValue(AO_9),epao.getChannelValue(AO_11));
	//memset(&thetemp,0,sizeof(epao));
	memcpy(thetemp, &epao, sizeof(epao));

	if(memcmp(thetemp,state,sizeof(epao))){
		out->SetPrio(prio);
		out->SetId(streamID);
		//memset(out->data,0,sizeof(epao));
		memcpy(&out->data, thetemp, sizeof(epao));
		//memset(&state,0,sizeof(epao));
		memcpy(state, thetemp, sizeof(epao));

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

void Ground::startRecieve(){
	while(true){
		receivePacket();
	}
}

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
