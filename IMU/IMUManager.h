/*
 * IMUManager.h
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */

#ifndef IMUMANAGER_H_
#define IMUMANAGER_H_


#include <string.h>
#include <stdlib.h>
#include <stdio.h> //Used for printf
#include <dirent.h>
#include <string>
#include <vector>
#include <tuple>
#include <map>
#include "rs232.h"


#define BAUD 500000
#define PATH "/dev/serial/by-id/"


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

	int init();
	imud readData();
	void setFilter(bool);

private:
	int devid;
	int offset_gyrox;
	int offset_gyroy;
	int offset_gyroz;
	int offset_accx;
	int offset_accy;
	int offset_accz;
	bool filter;
	imud imudata;


	/* Vector containing id and its measured offset for accl.
	 * in the order:
	 * <id from kernel>, <x_offset>, <y_offsett>, <z_offset>, <name>.
	 */
	std::vector<std::tuple<std::string,int,int,int,std::string>> dev_id = {
			std::make_tuple("A4001kB2",509,511,509,"IMU1"),
			std::make_tuple("AD01REIF",510,505,529,"IMU2")};

	/* Used to map IMU to comport with rs232. To add more
	/interfaces fill map as is in rs232.c
	 */
	std::map<std::string,int> devnr = {
			{"/dev/ttyUSB0",16},
			{"/dev/ttyUSB1",17},
			{"/dev/ttyUSB2",18},
			{"/dev/ttyUSB3",19},
			{"/dev/ttyUSB4",20},
			{"/dev/ttyUSB5",21},};

	//Timeout for serialinterface/IMU
	int IMU_timeout = 10;

	DIR           *d;
	struct dirent *dir;
	std::string full_device_name;
	std::string dev;

	int calibrate(int, int, int);
	void readImu();
	imud filterData(imud rawData);
	void setData(imud filteredData);
	int getDev();
};
}

#endif /* IMUMANAGER_H_ */
