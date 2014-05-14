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
int EBUPacketAnalogOut::setChannelValue(int volt,int  pin) {
	uint16_t data;
	if(volt > 5){
		data = 65535;
		toEBU.channel[pin] = data;
		return 0;
	} else if(volt < 0){
		data = 0;
		toEBU.channel[pin] = data;
		return 0;
	}
	data = ((float(volt)/5) * 65535);
	toEBU.channel[pin] = data;
	return 0;

}
uint16_t EBUPacketAnalogOut::getChannelValue(int pin) {
	return toEBU.channel[pin];
}


