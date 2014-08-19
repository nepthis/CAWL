#include "EBUPacketDigitalOut.h"
using namespace Packets;

EBUPacketDigitalOut::EBUPacketDigitalOut() {
	toEBU.channel[8] = ((uint8_t)0);
	destination = 0;
}

uint8_t EBUPacketDigitalOut::getDigitalOut(int channel){
	int byte = channel / 8;
	int offset = channel % 8;
	uint8_t mask = 0x01 << offset;
	return toEBU.channel[byte] & mask;
}

void EBUPacketDigitalOut::setDigitalOut( int channel, int value) {
	int byte = channel / 8;
	int offset = channel % 8;
	uint8_t mask = 0x01 << offset;
	toEBU.channel[byte] = (value)?(toEBU.channel[byte] | mask):(toEBU.channel[byte] & ~mask);
}

void EBUPacketDigitalOut::setDestination(int newDest) {
	destination = newDest;
}

int EBUPacketDigitalOut::getDestination() {
	return destination;
}

EBUPacketDigitalOut::~EBUPacketDigitalOut() {
}
