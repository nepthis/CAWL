/*
 * IMUManager.h
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */

#ifndef IMUMANAGER_H_
#define IMUMANAGER_H_

#include <stdlib.h>
#include <string>
#include <cstdlib>
#include <dirent.h>						//Used to get directory/path of IMU
#include <vector>						//Used for storing IMU meta-data
#include <tuple>						//Used for storing IMU meta-data
#include <map>							//Used for storing IMU meta-data
#include <mutex>						//Used to safely acces vars inbetween threads
#include <sstream>						//Used to manipuilate device path string
#include <algorithm>					//Used for sort
#include <cmath>						//Used for trigonometric functions etc.
#include "rs232.h"						//GPL library used for rs232 communication
#include <errno.h>						//Used to set error.nr
#include <cstring>						//Used for memcpy
#include "../Packets/ImuPack.h"			//Used to pack data for sim
#include "../logger.h"					//Used for logging
#include "../Globals.h"					//used for global signaling etc.

//#include <bitset>						//Not used anymore
//#include <iostream>					//Used while testing
//#include <thread>						//Not used anymore threading is outside of class

#define DAMPER 0.5						// real_tilt_angle*DAMPER, used to scale movement
#define MEDIANS 10						// Amount of samples

//
#define BAUD 500000						// Baudrate for rs232 set in IMU
#define PATH "/dev/serial/by-id/"		// Path to serial port in Ubuntu
#define BUFFER_SIZE 1024
#define IMU_TIMEOUT 500					// Timeout before polling rs232

//#define CALIBRATION_N 100
#define R_X 0
#define R_Y 1
#define R_Z 2

//ACC PARAMS
#define MAX_VOLTAGE 3.3					// Accelerometer specific params
#define SENSITIVITY 0.192
#define RESOLUTION_ACC 256

//LINEAR ACC FILTER
#define LIN_FILT 0.1

//GYRO ACC FILTER PARAMS
#define M_PI 3.14159265358979323846		// PI 3.14
#define GYRO_SCALE 80					// Gyroscope specific param
#define NEG_GYRO_Y -1
#define FILTER_WEIGHT 18				// Value between 5-20 as per description of filter
#define T 100				    		// frequency for IMU
#define DEG_TO_RAD 0.0174532925			// Used for conversion
#define TO_DEG 57.3

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
	IMUManager();//bool imu_rec, bool sim_snd
	virtual ~IMUManager();

	void setupImu();
	Packets::ImuPack getImuPack();
	bool isConnected();
	void readImu();


private:
	int getDev();
	int getxoffset(){return offset_accx;}
	int getyoffset(){return offset_accy;}
	int getzoffset(){return offset_accz;}

	void setFiltered(imud imudata);
	void getControl();

	imud setMedian(int16_t ax, int16_t ay, int16_t az, int16_t gx, int16_t gy, int16_t gz);
	void extractGyroAcc(char*);
	double gyroToFloat(int value){return (value/GYRO_SCALE)*(M_PI/180);}
	double accToFloat(int value){return value*(MAX_VOLTAGE/SENSITIVITY)/(RESOLUTION_ACC-1);}

	Packets::ImuPack imupack;
	Packets::ImuPack temppack;

	int medians;

	int16_t axv[MEDIANS];
	int16_t ayv[MEDIANS];
	int16_t azv[MEDIANS];
	int16_t gxv[MEDIANS];
	int16_t gyv[MEDIANS];
	int16_t gzv[MEDIANS];

	int16_t axvt[MEDIANS];
	int16_t ayvt[MEDIANS];
	int16_t azvt[MEDIANS];
	int16_t gxvt[MEDIANS];
	int16_t gyvt[MEDIANS];
	int16_t gzvt[MEDIANS];

	bool 			conn;

	int 			devid;
	int 			offset_accx;
	int 			offset_accy;
	int 			offset_accz;

	bool 			imuinit;
	int				imuinitn;

	double racc[3];
	double racc_prev[3];		//rav_l => result_acc_v_last
	double rest[3];

	double rgyro_prev[3];
	double rgyro[3];

	double linear_accx;
	double linear_accy;
	double linear_accz;

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
};
}

#endif /* IMUMANAGER_H_ */
