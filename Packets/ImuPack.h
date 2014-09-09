/*
 * ImuPack.h
 *
 *  Created on: Sep 9, 2014
 *      Author: cawl-server
 */

#ifndef IMUPACK_H_
#define IMUPACK_H_

#define GYRO_X 	0
#define GYRO_Y 	1
#define GYRO_Z 	2
#define ACC_X 		3
#define ACC_Y 		4
#define ACC_Z 		5

namespace Packets {
typedef struct sensorData {
	uint32_t packetId;
	uint32_t packetSize;
	float values[6];
	std::chrono::system_clock::time_point timeStamp;
}sensorData;
class ImuPack {
private:
	sensorData sens;
public:
	ImuPack();
	void setSensorDataValue(int data, float value);
	float	getSensorDataValue(int data);
	sensorData getSensorData();
	virtual ~ImuPack();
};

} /* namespace Packets */

#endif /* IMUPACK_H_ */
