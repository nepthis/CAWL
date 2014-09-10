/*
 * IMUManagere.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */
#include "IMUManager.h"

IMU::IMUManager::IMUManager() {
}

IMU::IMUManager::~IMUManager() {
}


/*
 * Used to see what IMU if any and to recieve it's
 * full devicename not the device path included
 * Checks dev_id vector for usable id's
 *
 *  Todo Cleanup less variables...
 *
 */
int IMU::IMUManager::getDev() {
	DIR           *d;
	struct dirent *dir;

	std::string full_device_name = NULL;
	std::string dev;
	char* dev_path = "/dev/serial/by-id/";

	d = opendir(dev_path);
	if(d){
		while((dir = readdir(d)) != NULL){
			dev = dir->d_name;
			for(int i = 0; i != dev_id.size(); ++i){
				if(dev.find(std::get<0>(dev_id[i]))){
					full_device_name = dev;
					calibrate(
							std::get<1>(dev_id[i]),
							std::get<2>(dev_id[i]),
							std::get<3>(dev_id[i]));
				}else{
					return 1;
				}
			}
		}
		std::stringstream ss;
		ss << dev_path << full_device_name;
		ss >> dev_path;
		return devnr.find(realpath(dev_path,NULL))->first;
	}else{
		return 1;
	}

}


/*
 * Start the interfacing the IMU
 */
int IMU::IMUManager::init() {
	while(devid<=0){
		devid = getDev();
		sleep(IMU_timeout);
	}
	return -1;

}

int IMU::IMUManager::calibrate(int x, int y , int z) {

	return -1;
}

IMU::imud IMU::IMUManager::readData() {
	return imudata;
}

void IMU::IMUManager::setFilter(bool filterOn) {
	filter = filterOn;
}

void IMU::IMUManager::readImu() {
}

IMU::imud IMU::IMUManager::filterData(imud rawData) {

	return rawData;
}

void IMU::IMUManager::setData(imud filteredData) {
}
