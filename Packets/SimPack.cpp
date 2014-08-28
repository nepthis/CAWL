/*
 * SimPack.cpp
 *
 *  Created on: May 20, 2014
 *      Author: cawl-server
 */

#include "SimPack.h"

namespace Packets {

SimPack::SimPack() {
	fromSim.packetId = 0;
	fromSim.packetSize = 0;
	fromSim.analog[8]= ((float)0);
	fromSim.digital = 0;
}

SimPack::~SimPack() {
	// TODO Auto-generated destructor stub
	//delete &fromSim;
}


void Packets::SimPack::setID(uint32_t id) {
	fromSim.packetId = id;
}

uint32_t Packets::SimPack::getPacketSize(void) {
	return fromSim.packetSize;
}

int Packets::SimPack::getDigital(int i) {
	return fromSim.digital & (1 << i)?1:0;
}

float Packets::SimPack::getAnalog(int i) {
	return fromSim.analog[i];
}

bool Packets::SimPack::operator ==(const SimPack& s) {
	return (fromSim.digital == s.fromSim.digital)
	& fromSim.analog[0] == s.fromSim.analog[0]
	& fromSim.analog[1] == s.fromSim.analog[1]
	& fromSim.analog[2] == s.fromSim.analog[2]
	& fromSim.analog[3] == s.fromSim.analog[3]
	& fromSim.analog[4] == s.fromSim.analog[4]
	& fromSim.analog[5] == s.fromSim.analog[5]
	& fromSim.analog[6] == s.fromSim.analog[6]
	& fromSim.analog[7] == s.fromSim.analog[7];
}

int Packets::SimPack::setDigital(int i, int value) {
	if(i < 32 && value == 1){
		fromSim.digital |= 1 << i;
		return 1;
	}else if(i < 32 && value == 0){
		fromSim.digital &= ~(1 << i);
		return 1;
	}else{
		return -1;
	}
}

int Packets::SimPack::setAnalog(int i, float value) {
	if(i < 8){
		fromSim.analog[i] = value;
		return 1;
	}else{
		return -1;
	}
}

float Packets::SimPack::fiveToOne(float value){
	return (value-0.5)/4.0;
}
float Packets::SimPack::fiveToOneNeg(float value){
	return (value-2.5)/2.5;
}
float Packets::SimPack::oneToFive(float value){
	return value*4.5;
}
float Packets::SimPack::oneToFiveNeg(float value){
	return (value+1)*2.25;
}
Packets::commandPacket Packets::SimPack::getData(void) {
 	return fromSim;
}
} /* namespace Packets */
