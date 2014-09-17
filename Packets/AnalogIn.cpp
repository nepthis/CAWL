/*
 * EBUPacketAnalogIn.cpp
 *
 *  Created on: Aug 25, 2014
 *      Author: cawl-server
 */

#include "AnalogIn.h"

namespace Packets {

AnalogIn::AnalogIn() {
	// TODO Auto-generated constructor stub

}

AnalogIn::~AnalogIn() {
	// TODO Auto-generated destructor stub
}

float Packets::AnalogIn::readPin(int pin) {
	//Convert uint16_t to voltage, 0.0-5.0 volt
}

void Packets::AnalogIn::setSource(int ebuNum) {
	source = ebuNum;
}

int Packets::AnalogIn::getSource() {
	return source;
}
} /* namespace Packets */
