#include "DigitalOut.h"
using namespace Packets;

DigitalOut::DigitalOut() {
	 edo.channel[8] = ((uint8_t)0);
	 destination = 0;
}

void DigitalOut::setDigitalOut(int channel, int value) {
	int byte = channel / 8;
	int offset = channel % 8;
	uint8_t mask = 0x01 << offset;
	edo.channel[byte] = (value)?(edo.channel[byte] | mask):(edo.channel[byte] & ~mask);
}

uint8_t DigitalOut::getDigitalOut( int channel) {
	int byte = channel / 8;
	int offset = channel % 8;
	uint8_t mask = 0x01 << offset;
	return edo.channel[byte] & mask;
}

EBUdigitalOut Packets::DigitalOut::getChannel() {
	return edo;
}

void Packets::DigitalOut::setDestination(int dest) {
	destination = dest;
}

int Packets::DigitalOut::getDestination() {
	return destination;
}

DigitalOut::~DigitalOut() {
}
