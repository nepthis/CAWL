/*
 * IMUManagere.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */
#include "IMUManager.h"

std::mutex imupack_lock;

namespace IMU{

IMUManager::IMUManager(){
	imuinit = true;
	conn = false;
	bufsize = imuinitn = medians = 0;
	offset_accx = offset_accy = offset_accz = 0;
	linear_accx = linear_accy = linear_accz = 0;
	d = NULL;
	dir = NULL;
	devid = -1;


}

/*
 * Start interfacing the IMU using rs232
 */
void IMUManager::setupImu(){
	while(devid<0){
		devid = getDev();
		if(devid == -1){
			sleep(5);
		}
	}
	if(RS232_OpenComport(devid, BAUD)) {
		logError("IMUManager: Could not connect to comport ");
		logVerbose("IMUManager: Be sure to start application in as root/sudo. Or make application member of dialout.");
		errno = ECANCELED;
		throw errno;
	}else{
		conn = true;
	}
}


/*
 * Used to see what IMU if any is connected and to recieve it's
 * full devicename not the device path included
 * Checks dev_id vector for usable id's. Also sets stored offsets
 * for IMU
 *
 */
int IMUManager::getDev() {

	DIR           *d;
	struct dirent *dir;

	std::string full_device_name = "";
	char* dev;
	char* dev_path = (char*)"/dev/serial/by-id/";
	d = opendir(dev_path);

	if(d != NULL){
		dir = readdir(d);
		while(dir != NULL){
			dev = dir->d_name;
			dir = readdir(d);

			for(int i = 0; i != dev_id.size(); ++i){
				std::string tmp = std::get<0>(dev_id[i]);
				std::string devtmp(dev);
				std::size_t f = devtmp.find(tmp);
				if(static_cast<int>(f) != -1){
					full_device_name = devtmp;
					offset_accx = std::get<1>(dev_id[i]);
					offset_accy = std::get<2>(dev_id[i]);
					offset_accz = std::get<3>(dev_id[i]);
				}
			}
		}
		closedir(d);
		std::stringstream ss;
		ss << dev_path << full_device_name.c_str();
		ss >> full_device_name;

		return devnr.find(realpath(full_device_name.c_str(),NULL))->second;
	}else{
		closedir(d);
		return -1;
	}
}

/*
 * Returns the imupack containing data tilt, and placement data
 * needed by the oryx-platform
 */

Packets::ImuPack IMUManager::getImuPack(){
	imupack_lock.lock();
	return imupack;
	imupack_lock.unlock();
}


/*
 * return bool to check if connected to IMU
 */
bool IMUManager::isConnected(){
	return conn;
}


/*
 * Used to get a 22 byte command from the IMU
 */
void IMUManager::readImu() {
	int n,t=0;
	unsigned char b;
	unsigned char buf[22];

	while(not signaled){
		usleep(IMU_TIMEOUT);
		for(int i = 0; i < 5;++i){
			memset(&buf,0,22);
			n = RS232_PollComport(devid, buf, 1);
			if(n!=0 && buf[0]!=0x00){
				i = 0;
			}
			if(n==0 && i>0){
				i--;
				usleep(IMU_TIMEOUT);
			}
		}
		memset(&buf,0,22);


		//will poll comport until a 22 byte command is recieved
		while(1){
			n = RS232_PollComport(devid, buf+t, 22-t);
			t=t+n;

			if(t==22 && memcmp((char*)buf+18,(char*)"\x00\x00\x00\x00",4)==0){
				t = n = 0;

				// Used to extract acc and gyro from buffered command
				extractGyroAcc((char*)buf);
				memset(&buf,0,22);
			}else if(t>22){
				t = n = 0;
				memset(&buf,0,22);
				break;
			}else if(t==22 && memcmp((char*)buf+18,(char*)"\x00\x00\x00\x00",4)!=0){
				t = n = 0;
				memset(&buf,0,22);
				break;
			}
			usleep(IMU_TIMEOUT);
		}
	}
}


/*
 * Used to ectract Gyro and acc from the 22 byte command
 */
void IMUManager::extractGyroAcc(char* command) {

	uint32_t gytemp;
	int16_t  actemp;

	//Set gyrox,-y,-z from command (Gyros)
	memmove(&gytemp,command,4);
	gytemp = __builtin_bswap32(gytemp);
	gytemp = (gytemp & 0x3FFFC00) >> 10;
	int16_t gyrox = 0xffff&gytemp;

	memmove(&gytemp,command+4,4);
	gytemp = __builtin_bswap32(gytemp);
	gytemp = (gytemp & 0x3FFFC00) >> 10;
	int16_t gyroy = 0xffff&gytemp;

	memmove(&gytemp,command+8,4);
	gytemp = __builtin_bswap32(gytemp);
	gytemp = (gytemp & 0x3FFFC00) >> 10;
	int16_t gyroz = 0xffff&gytemp;

	//Set accx,-y,-z from  command (accelerometers) 9 bit resolution
	memmove(&actemp,command+12,2);
	actemp = __builtin_bswap16(actemp);
	int16_t accy = (actemp - getxoffset()) >> 2;

	memmove(&actemp,command+14,2);
	actemp = __builtin_bswap16(actemp);
	int16_t accx = (actemp - getyoffset()) >> 2;

	memmove(&actemp,command+16,2);
	actemp = __builtin_bswap16(actemp);
	int16_t accz = (actemp - getzoffset()) >> 2;

	imud imudata = setMedian(accx, accy, accz, gyrox, gyroy*NEG_GYRO_Y, gyroz);

	setFiltered(imudata);
}


/*
 * Used to set median value of input command
 */
imud IMUManager::setMedian(int16_t ax, int16_t ay, int16_t az,
		int16_t gx, int16_t gy, int16_t gz) {
	if(medians < MEDIANS){
		axv[medians] = ax;
		ayv[medians] = ay;
		azv[medians] = az;
		gxv[medians] = gx;
		gyv[medians] = gy;
		gzv[medians] = gz;
		medians++;
	}else{
		memcpy(axvt,axv,sizeof(axv));
		memcpy(ayvt,ayv,sizeof(ayv));
		memcpy(azvt,azv,sizeof(azv));
		memcpy(gxvt,gxv,sizeof(gxv));
		memcpy(gyvt,gyv,sizeof(gyv));
		memcpy(gzvt,gzv,sizeof(gzv));

		memcpy(axv,axvt+1,sizeof(axv));
		memcpy(ayv,ayvt+1,sizeof(ayv));
		memcpy(azv,azvt+1,sizeof(azv));
		memcpy(gxv,gxvt+1,sizeof(gxv));
		memcpy(gyv,gyvt+1,sizeof(gyv));
		memcpy(gzv,gzvt+1,sizeof(gzv));
		axv[medians-1] = ax;
		ayv[medians-1] = ay;
		azv[medians-1] = az;
		gxv[medians-1] = gx;
		gyv[medians-1] = gy;
		gzv[medians-1] = gz;
	}

	memcpy(axvt,axv,sizeof(axv));
	memcpy(ayvt,ayv,sizeof(ayv));
	memcpy(azvt,azv,sizeof(azv));
	memcpy(gxvt,gxv,sizeof(gxv));
	memcpy(gyvt,gyv,sizeof(gyv));
	memcpy(gzvt,gzv,sizeof(gzv));

	std::sort(axvt,axvt+medians);
	std::sort(ayvt,ayvt+medians);
	std::sort(azvt,azvt+medians);
	std::sort(gxvt,gxvt+medians);
	std::sort(gyvt,gyvt+medians);
	std::sort(gzvt,gzvt+medians);

	imud imudata;

	imudata.accx  = accToFloat(axvt[medians/2]);
	imudata.accy  = accToFloat(ayvt[medians/2]);
	imudata.accz  = accToFloat(azvt[medians/2]);
	imudata.gyrox = gyroToFloat(gxvt[medians/2]);
	imudata.gyroy = gyroToFloat(gyvt[medians/2]);
	imudata.gyroz = gyroToFloat(gzvt[medians/2]);

	return imudata;
}


/*
 * Used to extrapolate gyro and use acc to get angles, and linear acc.
 * Needs to be rewritten.
 */
void IMUManager::setFiltered(imud imudata) {

	double accx  = imudata.accx;
	double accy  = imudata.accy;
	double accz  = imudata.accz;

	double gyrox = imudata.gyrox;
	double gyroy = imudata.gyroy;
	double gyroz = imudata.gyroz;

	double raccxn, raccyn, racczn;

	float raccl = sqrt( pow(accx,2.0) + pow(accy,2.0) + pow(accz,2.0));
	raccxn = accx/raccl;
	raccyn = accy/raccl;
	racczn = accz/raccl;

	double raccxnd = acos(raccxn)*57.3;
	double raccynd = acos(raccyn)*57.3;
	double raccznd = acos(racczn)*57.3;

	double rxgyro;
	double rygyro;
	double rzgyro;

	//Set initial acc values
	if(imuinit){
		rest[R_X] = raccxn;
		rest[R_Y] = raccyn;
		rest[R_Z] = racczn;

		linear_accx = 0;
		linear_accy = 0;
		linear_accz = 0;

		imuinit = false;
	}
	if(fabs(rest[R_Z]) > 0.1){
		rgyro[R_X]= gyrox;
		rgyro[R_Y]= gyroy;
		rgyro[R_Z]= gyroz;

		double axz_prev = atan2(rest[R_X],rest[R_Z]);
		double ayz_prev = atan2(rest[R_Y],rest[R_Z]);

		double axz = axz_prev + rgyro[R_X]*(1/T);
		double ayz = ayz_prev + rgyro[R_Y]*(1/T);

		rxgyro = sin(axz) / sqrt(1 + pow(cos(axz),2.0) * pow(tan(ayz),2.0));
		rygyro = sin(ayz) / sqrt(1 + pow(cos(ayz),2.0) * pow(tan(axz),2.0));
		double rzgyrotemp = sqrt(1 - pow(rxgyro,2.0) - pow(rygyro,2.0));
		rzgyro = rzgyrotemp>=0?rzgyrotemp:rzgyrotemp*-1;
	}else{
		rxgyro = rest[R_X];
		rygyro = rest[R_Y];
		rzgyro = rest[R_Z];
	}
	rest[R_X] = (raccxn + rxgyro * FILTER_WEIGHT ) / (1 + FILTER_WEIGHT);
	rest[R_Y] = (raccyn + rygyro * FILTER_WEIGHT ) / (1 + FILTER_WEIGHT);
	rest[R_Z] = (racczn + rzgyro * FILTER_WEIGHT ) / (1 + FILTER_WEIGHT);

	double r = sqrt(pow(rest[R_X],2.0) + pow(rest[R_Y],2.0) +  pow(rest[R_Z],2.0));

	rest[R_X] = rest[R_X]/r;
	rest[R_Y] = rest[R_Y]/r;
	rest[R_Z] = rest[R_Z]/r;

	double roll = (acos(rest[R_X]) - M_PI / 2)*DAMPER;
	double pitch= (acos(rest[R_Y]) - M_PI / 2)*DAMPER;

	// set imupack
	temppack.setSensorDataValue(ROLL,
			fabs(roll) > M_PI/6 ?
					copysign(M_PI/6,roll) :
					roll);

	temppack.setSensorDataValue(PITCH,
			fabs(pitch)>M_PI/6 ?
					copysign(M_PI/6,pitch) :
					pitch);

	temppack.setSensorDataValue(YAW,0);

	linear_accx = (raccxn - rest[R_X]);
	linear_accy = linear_accx*LIN_FILT + (raccyn - rest[R_Y])*(1-LIN_FILT);
	linear_accz = linear_accx*LIN_FILT + (racczn - rest[R_Z])*(1-LIN_FILT);

	temppack.setSensorDataValue(HEAVE,0);
	temppack.setSensorDataValue(SWAY,0);
	temppack.setSensorDataValue(SURGE,0);
	temppack.stampTime();

	imupack_lock.lock();
	imupack = temppack;
	imupack_lock.unlock();

}


IMUManager::~IMUManager() {
}

}
