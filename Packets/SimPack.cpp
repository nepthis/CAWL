/*
 * SimPack.cpp
 *
 *  Created on: May 20, 2014
 *      Author: cawl-server
 */

#include "SimPack.h"

namespace Packets {

SimPack::SimPack() {
	fromSim = commandPacket{0,0,0,0,0,0,0,0,0,0,0,0};

}

SimPack::~SimPack() {
	// TODO Auto-generated destructor stub
}

} /* namespace Packets */

void Packets::SimPack::setID(uint32_t id) {
	fromSim.packetId = id;
}

uint32_t Packets::SimPack::getPacketSize(void) {
	return fromSim.packetSize;
}
 commandPacket Packets::SimPack::getData(void) {
	return fromSim;
}
