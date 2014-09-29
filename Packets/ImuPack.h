/*
 * ImuPack.h
 *
 *  Created on: Sep 9, 2014
 *      Author: cawl-server
 */

#ifndef IMUPACK_H_
#define IMUPACK_H_

#include <chrono>
#include <cstdint>

//meters
#define SURGE 		0
#define SWAY 		1
#define HEAVE 		2

//radians
#define ROLL 		3
#define PITCH 		4
#define YAW		 	5


namespace Packets {
typedef struct sensorData {
	uint32_t packetId;
	uint32_t packetSize;
	float values[6];
	std::chrono::system_clock::time_point timeStamp;
}sensorData;

class ImuPack {

public:
	ImuPack();
	sensorData sens;
	void setSensorDataValue(int data, float value);
	float	getSensorDataValue(int data);
	sensorData getSensorData();
	void stampTime();
	virtual ~ImuPack();
};

} /* namespace Packets */

#endif /* IMUPACK_H_ */
