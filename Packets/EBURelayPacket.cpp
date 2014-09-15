/*
 * EBURelayPacket.cpp
 *
 *  Created on: May 5, 2014
 *  Author: Robin Bond
 *  Desc: class for the struct that contains the array for the relays + defines for using shorthand names
 *  for the different relays. Most of this is borrowed from the Wheel Loader Project group.
 */

#include "EBURelayPacket.h"
using namespace Packets;

EBURelayPacket::EBURelayPacket() {
	er.channel[14]= (0,0,0,0,0,0,0,0,0,0,0,0,0,0);
	destination = 0;
}

EBURelayPacket::~EBURelayPacket() {
}

EBUrelays EBURelayPacket::getRelays(void) {
	return er;
}
/*	setRelayVAlue sets a relay to on or off, two relays are stored per byte.
 * 	The defines in the header makes it easier to know which position to set
 * 	on or off.
 */
void EBURelayPacket::setRelayValue(int relay, int value) {
	int byte = relay / 8;
	int offset = relay % 8;
	uint16_t mask = 0x01 << offset;
	er.channel[byte] = (value)?(er.channel[byte] | mask):(er.channel[byte] & ~mask);
}

//	Same as the setRelayValue except this one only reads the value
uint8_t EBURelayPacket::getRelayValue( int relay) {
	int byte = relay / 8;
	int offset = relay % 8;
	uint8_t mask = 0x01 << offset;
	uint8_t result = er.channel[byte] & mask;
	return result >>offset;
}
