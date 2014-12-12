/*
 * SimPack.cpp
 *  Created on: May 20, 2014
 *  Author: Robin Bond & H�kan Ther�n
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */

#include "SimPack.h"

namespace Packets {

SimPack::SimPack() {
	fs.packetId = 0;
	fs.packetSize = 44;
	fs.analog[8]= ((float)0);
	fs.digital = 0;
}

SimPack::~SimPack() {
	// TODO Auto-generated destructor stub
	//delete &fs;
}


void Packets::SimPack::setID(uint32_t id) {
	fs.packetId = id;
}

uint32_t Packets::SimPack::getPacketSize(void) {
	return fs.packetSize;
}

int Packets::SimPack::getDigital(int i) {
	return fs.digital & (1 << i)?1:0;
}

float Packets::SimPack::getAnalog(int i) {
	return fs.analog[i];
}

bool Packets::SimPack::operator ==(const SimPack& s) {
	return (fs.digital == s.fs.digital)
			& fs.analog[0] == s.fs.analog[0]
										  & fs.analog[1] == s.fs.analog[1]
																		& fs.analog[2] == s.fs.analog[2]
																									  & fs.analog[3] == s.fs.analog[3]
																																	& fs.analog[4] == s.fs.analog[4]
																																								  & fs.analog[5] == s.fs.analog[5]
																																																& fs.analog[6] == s.fs.analog[6]
																																																							  & fs.analog[7] == s.fs.analog[7];
}

int Packets::SimPack::setDigital(int i, int value) {
	if(i < 32 && value == 1){
		fs.digital |= 1 << i;
		return 1;
	}else if(i < 32 && value == 0){
		fs.digital &= ~(1 << i);
		return 1;
	}else{
		return -1;
	}
}

int Packets::SimPack::setAnalog(int i, float value) {
	if(i < 8){
		fs.analog[i] = value;
		return 1;
	}else{
		return -1;
	}
}

fromSim Packets::SimPack::getData(void) {
	return fs;
}

} /* namespace Packets */
