/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: cawl-server
 */

#include "Ground.h"
using namespace std;

Ground::Ground(Netapi::CawlSocket* gw_socket) {
	countBoomUp = countBuckUp = countBoomDown = countBuckDown = 0;
	socketOut =  gw_socket;
	sp =  Packets::SimPack();
	epao = Packets::EBUPacketAnalogOut();

	thetemp = (char*) malloc(sizeof(epao));
	state = (char*) malloc(sizeof(epao));

	simulator = new Simulator::Sim();
	out = new Packets::CawlPacket();
	in = new Packets::CawlPacket();
}

void Ground::sendPacket() {
	int prio = 1;			// <---- borde sättas beroende av paket
	int streamID = 1;		// <----
	sp = simulator->recPac();
	setEbuOne(&sp, &epao);
	memset(&thetemp,0,sizeof(epao));
	memcpy(&thetemp, &epao, sizeof(epao));

	if(thetemp != state){
		out->SetPrio(prio);
		out->SetId(streamID);
		memset(out->data,0,sizeof(epao));
		memcpy(out->data, thetemp, sizeof(epao));
		memset(&state,0,sizeof(epao));
		memcpy(&state, &thetemp, sizeof(epao));

		//LÅS MUTEX ETC!!!!
		m_cawlSocket.lock();
		try{
			socketOut->send(*out);
		}catch(int e){
			errno = EREMOTEIO;
			throw 0;
		}
		m_cawlSocket.unlock();
	}
}



void Ground::receivePacket(){
	m_cawlSocket.lock();
	try{
		socketOut->rec(*out);
	}catch(int e){
		errno = EREMOTEIO;
		throw 0;
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
		Ground::receivePacket();
		usleep(100);
	}
}

void Ground::startSend(){
	while(true)
		Ground::sendPacket();
		usleep(100);
}

Ground::~Ground() {
	delete simulator;
	delete out;
	delete in;
}
