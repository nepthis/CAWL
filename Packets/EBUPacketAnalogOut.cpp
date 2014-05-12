/*
 * EBUPacketAnalogOut.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: Robin Bond
 */

#include "EBUPacketAnalogOut.h"
using namespace Packets;

EBUPacketAnalogOut::EBUPacketAnalogOut() {
	// TODO Auto-generated constructor stub
	toEBU.channel[24] = {0};

}

EBUPacketAnalogOut::~EBUPacketAnalogOut() {
	// TODO Auto-generated destructor stub
}

ebuAnOut EBUPacketAnalogOut::getChannel() {
	return toEBU;
}

float EBUPacketAnalogOut::getChannelValue(int pin) {
	float voltage;
	uint8_t data = toEBU.channel[pin];
	voltage = (((float)data/65535) * 5);
	return voltage;
}

void EBUPacketAnalogOut::setChannelValue(int volt,int  pin) {
	uint16_t data;
	if(volt >= 5){
		data = 65535;
	} else if(volt <= 0){
		data = 0;
	}else{
		data = ((volt/5) * 65535 + 0.5);
	}
	toEBU.channel[pin] = data;
}
