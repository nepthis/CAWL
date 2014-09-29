/*
 * IMUManager.h
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */

#ifndef IMUMANAGER_H_
#define IMUMANAGER_H_

#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <mutex>
#include <thread>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <bitset>
#include "rs232.h"
#include "../Packets/ImuPack.h"

#define BAUD 500000
#define PATH "/dev/serial/by-id/"
#define BUFFER_SIZE 1024
#define IMU_TIMEOUT 500
#define FILTER_RATIO 0.1
#define CALIBRATION_N 100
#define R_X 0
#define R_Y 1
#define R_Z 2
#define TO_DEG 57.3



//ACC PARAMS
#define MAX_VOLTAGE 3.3
#define SENSITIVITY 0.192
#define RESOLUTION_ACC 512

//GYRO PARAMS
#define M_PI 3.14159265358979323846
#define GYRO_SCALE 80
#define NEG_GYRO_Y -1
#define FILTER_WEIGHT 13		//Value between 5-20 as per description of filter
#define T 1/100				//frequence for IMU

namespace IMU{

typedef struct imud{
	float gyrox;
	float gyroy;
	float gyroz;
	float accx;
	float accy;
	float accz;
}imud;

class IMUManager{

public:
	IMUManager();
	virtual ~IMUManager();
	Packets::ImuPack imupack;

	bool connected(){return conn;}
	bool 			conn;

private:
	int 			devid;
	int 			offset_accx;
	int 			offset_accy;
	int 			offset_accz;

	int16_t 		gyrox;
	int16_t 		gyroy;
	int16_t 		gyroz;
	int16_t 		accx;
	int16_t 		accy;
	int16_t 		accz;

	uint32_t 		gytemp;
	int16_t 		actemp;

	bool 			imuinit;
	int				imuinitn;

	double racc[3];
	double racc_prev[3];		//rav_l => result_acc_v_last
	double rest[3];

	double rgyro_prev[3];
	double rgyro[3];

	imud 			imudata;
	imud 			old;

	unsigned char 	buf[BUFFER_SIZE];
	int 			bufsize;

	DIR           	*d;
	struct dirent 	*dir;
	std::string   	full_device_name;
	std::string 	dev;

	/* Vector containing id and its measured offset for accl.
	 * in the order:
	 * <id from kernel>, <x_offset>, <y_offsett>, <z_offset>, <name>.
	 */
	std::vector<std::tuple<std::string,int,int,int,std::string>> dev_id = {
			std::make_tuple("A4001kB2",509,511,509,"IMU1"),
			std::make_tuple("AD01REIF",509,505,532,"IMU2")};  //red led, offset/bias set 2014-09-23

	/* Used to map IMU to comport with rs232. To add more
	/interfaces fill map as is in rs232.c, for the moment
	 * only USB.
	 */
	std::map<std::string,int> devnr = {
			{"/dev/ttyUSB0",16},
			{"/dev/ttyUSB1",17},
			{"/dev/ttyUSB2",18},
			{"/dev/ttyUSB3",19},
			{"/dev/ttyUSB4",20},
			{"/dev/ttyUSB5",21},};

	int init();
	int getDev();
	int getxoffset(){return offset_accx;}
	int getyoffset(){return offset_accy;}
	int getzoffset(){return offset_accz;}

	void setAngles(float accx, float accy, float accz, float gyrox, float gyroy, float gyroz);
	void getControl();

	void calibrate(int x, int y , int z);
	void readImu();
	void filterData();
	void setData(char*);

	float gyroToFloat(int value){return (value/GYRO_SCALE)*(M_PI/180);}
	float accToFloat(int value){return value*(MAX_VOLTAGE/SENSITIVITY)/(RESOLUTION_ACC-1);}
};
}

#endif /* IMUMANAGER_H_ */
