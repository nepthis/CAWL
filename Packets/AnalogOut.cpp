/*
 * EBUPacketAnalogOut.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: Robin Bond
 */

#include "AnalogOut.h"
using namespace Packets;

AnalogOut::AnalogOut() {
	// TODO Auto-generated constructor stub
	toEBU.channel[24] = ((uint16_t)0);
	destination = 0;

}

AnalogOut::~AnalogOut() {
	// TODO Auto-generated destructor stub
}

ebuAnOut AnalogOut::getChannel() {
	return toEBU;
}
/*	If the value is too high or too low maximum/minimum values will be set.
 * 	The volt should range between 0-5 and will be translated into a value for a 16bit
 * 	integer where lowest value 0V will equal 0 and the highest value of 5V will equal
 * 	65535.
 */
int AnalogOut::setChannelValue(float volt,int  pin) {
	uint16_t data;
	if(volt > 5.0){
		data = 65535;
		toEBU.channel[pin] = data;
		return 0;
	} else if(volt < 0.0){
		data = 0;
		toEBU.channel[pin] = data;
		return 0;
	}
	data =(uint16_t) ((volt/5) * 65535 +0.5); //the 0.5 is here because conversion from float-int
	toEBU.channel[pin] = data;
	return 0;

}
uint16_t AnalogOut::getChannelValue(int pin) {
	return toEBU.channel[pin];
}

int Packets::AnalogOut::getDestination(void) {
	return destination;
}

void Packets::AnalogOut::setDestination(int dest) {
	destination = dest;
}
