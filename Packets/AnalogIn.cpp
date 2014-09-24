/*
 * EBUPacketAnalogIn.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: cawl-server
 */

#include "AnalogIn.h"

namespace Packets {

AnalogIn::AnalogIn() {
	source = 0;
}

AnalogIn::~AnalogIn() {

}
//Borrowed from other project and modified slightly
float Packets::AnalogIn::readPin(int pin) {
	//Convert uint16_t to voltage, 0.0-5.0 volt
	uint16_t intValue = ed.channel[pin];
	float value = (((float)intValue/4095) * 5);
	return value;
}

void Packets::AnalogIn::setSource(int ebuNum) {
	source = ebuNum;
}

int Packets::AnalogIn::getSource() {
	return source;
}
} /* namespace Packets */
