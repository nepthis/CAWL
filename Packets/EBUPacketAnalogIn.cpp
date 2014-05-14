/*
 * EBUPacket.cpp
 *
 *  Created on: Apr 24, 2014
 *      Author: cawl-server
 */

#include "EBUPacketAnalogIn.h"
using namespace Packets;

EBUPacketAnalogIn::EBUPacketAnalogIn() {
	// TODO Auto-generated constructor stub
	fromEBU.channel[24] = {0};

}

EBUPacketAnalogIn::~EBUPacketAnalogIn() {
	// TODO Auto-generated destructor stub
}

ebuAnIn EBUPacketAnalogIn::getChannel(void) {
	return fromEBU;
}

void 	EBUPacketAnalogIn::setChannelValue(int pin, int volt) {
	uint16_t data;
	if(volt >= 5){
		data = 65535;
	} else if(volt <= 0){
		data = 0;
	}else{
		data = ((volt/5) * 65535 + 0.5);
	}
	fromEBU.channel[pin] = data;
}

float EBUPacketAnalogIn::getChannelValue(int pin) {
	float voltage;
	uint8_t data = fromEBU.channel[pin];
	voltage = (((float)data/65535) * 5);
	return voltage;
}
