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
	fromSim.analog[9]= ((float)0);
	fromSim.digital = 0;
}

SimPack::~SimPack() {
	// TODO Auto-generated destructor stub
	//delete &fromSim;
}

} /* namespace Packets */

void Packets::SimPack::setID(uint32_t id) {
	fromSim.packetId = id;
}

uint32_t Packets::SimPack::getPacketSize(void) {
	return fromSim.packetSize;
}
 Packets::commandPacket Packets::SimPack::getData(void) {
	return fromSim;
}
