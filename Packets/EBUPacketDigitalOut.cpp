#include "EBUPacketDigitalOut.h"
using namespace Packets;

EBUPacketDigitalOut::EBUPacketDigitalOut() {
	 edo.channel[8] = ((uint8_t)0);
	 destination = 0;
}

void EBUPacketDigitalOut::setDigitalOut(int channel, int value) {
	int byte = channel / 8;
	int offset = channel % 8;
	uint8_t mask = 0x01 << offset;
	edo.channel[byte] = (value)?(edo.channel[byte] | mask):(edo.channel[byte] & ~mask);
}

uint8_t EBUPacketDigitalOut::getDigitalOut( int channel) {
	int byte = channel / 8;
	int offset = channel % 8;
	uint8_t mask = 0x01 << offset;
	return edo.channel[byte] & mask;
}

EBUdigitalOut Packets::EBUPacketDigitalOut::getChannel() {
	return edo;
}

void Packets::EBUPacketDigitalOut::setDestination(int dest) {
	destination = dest;
}

int Packets::EBUPacketDigitalOut::getDestination() {
	return destination;
}

EBUPacketDigitalOut::~EBUPacketDigitalOut() {
}
