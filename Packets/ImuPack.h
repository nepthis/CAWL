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
#define ACC_X 		0
#define ACC_Y 		1
#define ACC_Z 		2
#define GYRO_X 	3
#define GYRO_Y 	4
#define GYRO_Z 	5


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
