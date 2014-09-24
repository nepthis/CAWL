/*
 * DigitalIn.cpp
 *
 *  Created on: Sep 17, 2014
 *      Author: cawl-server
 */

#include "DigitalIn.h"

namespace Packets {

DigitalIn::DigitalIn() {
	source = 0;

}
//Borrowed from wheel loader project and modified
void DigitalIn::setChannelValue(int value, int pin) {
	int byte = pin / 8;
	int offset = pin % 8;
	uint8_t mask = 0x01 << offset;
	did.channel[byte] = (value)?(did.channel[byte] | mask):(did.channel[byte] & ~mask);
}
//Borrowed from wheel loader project and modified
float DigitalIn::getChannelValue(int pin) {
	int byte = pin / 8;
	int offset = pin % 8;
	uint8_t mask = 0x01 << offset;
	return did.channel[byte] & mask;
}

DigitalInData DigitalIn::getChannel() {
	return did;
}

void DigitalIn::setSource(int ebuNum) {
	source = ebuNum;
}

int DigitalIn::getSource() {
	return source;
}

DigitalIn::~DigitalIn() {
	// TODO Auto-generated destructor stub
}

} /* namespace Packets */
