/*
 * ImuPack.cpp
 *
 *  Created on: Sep 9, 2014
 *      Author: cawl-server
 */

#include "ImuPack.h"

namespace Packets {

ImuPack::ImuPack() {
	sens.values[6]= ((float)0);
}

void ImuPack::setSensorDataValue(int data, float value) {
	sens.values[data] = value;
}

float ImuPack::getSensorDataValue(int data) {
	return sens.values[data];
}

sensorData ImuPack::getSensorData() {
	return sens;
}

void ImuPack::stampTime() {
	sens.timeStamp = std::chrono::high_resolution_clock::now();
}

ImuPack::~ImuPack() {
}

} /* namespace Packets */
