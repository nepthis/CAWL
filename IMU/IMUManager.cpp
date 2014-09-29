/*
 * IMUManagere.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */
#include "IMUManager.h"


std::mutex data_lock;

IMU::IMUManager::IMUManager() {
	imuinit = true;
	conn = false;
	offset_accx = 0;
	offset_accy = 0;
	offset_accz = 0;
	bufsize = 0;
	devid = -1;
	old.accx = 0;
	old.accy = 0;
	old.accz = 0;
	old.gyrox = 0;
	old.gyroy = 0;
	old.gyroz = 0;
	imuinitn = 0;

	init();
}


/*
 * Start the interfacing the IMU
 */
int IMU::IMUManager::init() {
	while(devid<0){
		devid = getDev();
		if(devid == -1){sleep(5);}

	}

	if(RS232_OpenComport(devid, BAUD)) {
		return 1;
	}else{
		conn = true;
	}

	std::thread t1 (&IMU::IMUManager::readImu, this);
	std::thread t2 (&IMU::IMUManager::getControl , this);

	t1.detach();
	t2.detach();

	//Todo Start threads for reading the comport
	while(1){
		//printf("%s\n",(char*)buf);
		sleep(1);
	}
	return -1;
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
					calibrate(
							std::get<1>(dev_id[i]),
							std::get<2>(dev_id[i]),
							std::get<3>(dev_id[i]));
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
 * ?
 */
void IMU::IMUManager::calibrate(int x, int y , int z) {
	offset_accx = x;
	offset_accy = y;
	offset_accz = z;
}

/*
 *?
 */
void IMU::IMUManager::readImu() {
	int n,t=0;
	unsigned char b;
	unsigned char buf[22];
	unsigned char buftemp[22];

	while(1){
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

		while(1){
			n = RS232_PollComport(devid, buf+t, 22-t);
			t=t+n;
			uint32_t gyrox = 0;

			/* Spår utskrift för bitrepresentaiton av kommandon
			const char* beg = reinterpret_cast<const char*>(&buf);
			const char* end = beg + sizeof(buf);
			while(beg != end){
				std::cout << std::bitset<CHAR_BIT>(*beg++);
			}std::cout << std::endl;
			 */
			if(t==22 && memcmp((char*)buf+18,(char*)"\x00\x00\x00\x00",4)==0){
				t = n = 0;
				setData((char*)buf);
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


void IMU::IMUManager::setData(char* command) {

	//Set gyrox,y,z from command (Gyros)
	memmove(&gytemp,command,4);
	gytemp = __builtin_bswap32(gytemp);
	gytemp = (gytemp & 0x3FFFC00) >> 10;
	gyrox = 0xffff&gytemp;

	memmove(&gytemp,command+4,4);
	gytemp = __builtin_bswap32(gytemp);
	gytemp = (gytemp & 0x3FFFC00) >> 10;
	gyroy = 0xffff&gytemp;

	memmove(&gytemp,command+8,4);
	gytemp = __builtin_bswap32(gytemp);
	gytemp = (gytemp & 0x3FFFC00) >> 10;
	gyroz = 0xffff&gytemp;

	//Set accx,y,z from  command (accelerometers) 9 bit resolution
	memmove(&actemp,command+12,2);
	actemp = __builtin_bswap16(actemp);
	accy = (actemp - getxoffset()) >> 1;

	memmove(&actemp,command+14,2);
	actemp = __builtin_bswap16(actemp);
	accx = (actemp - getyoffset()) >> 1;

	memmove(&actemp,command+16,2);
	actemp = __builtin_bswap16(actemp);
	accz = (actemp - getzoffset()) >> 1;


	data_lock.lock();
	imudata.gyrox = gyroToFloat(gyrox);
	imudata.gyroy = gyroToFloat(gyroy*NEG_GYRO_Y);//*NEG_GYRO_Y; //Inverted placement on board
	imudata.gyroz = gyroToFloat(gyroz);
	imudata.accx  = accToFloat(accx);
	imudata.accy  = accToFloat(accy);
	imudata.accz  = accToFloat(accz);
	filterData();
	data_lock.unlock();
}

/*
 * ?
 */
void IMU::IMUManager::getControl() {
	while(1){
		data_lock.lock();
		float accx = imudata.accx;
		float accy = imudata.accy;
		float accz = imudata.accz;
		float gyrox= imudata.gyrox;
		float gyroy= imudata.gyroy;
		float gyroz= imudata.gyroz;
		data_lock.unlock();

		Packets::ImuPack p = Packets::ImuPack();
		p.setSensorDataValue(GYRO_X,gyrox);
		p.setSensorDataValue(GYRO_Y,gyroy);
		p.setSensorDataValue(GYRO_Z,gyroz);
		p.setSensorDataValue(ACC_X,accx);
		p.setSensorDataValue(ACC_Y,accy);
		p.setSensorDataValue(ACC_Z,accz);
		p.stampTime();

		setAngles(accx,accy,accz,gyrox,gyroy,gyroz);
		usleep(1000000/10000);


		//Spårutskrifter
		//usleep(50000);
		//std::cout << "X: "<< accx << " Y: " << accy << " Z: " << accz << std::endl;
		//std::cout << "X: "<<gyrox << " Y: " <<gyroy << " Z: " <<gyroz << std::endl;
		//std::cout << std::endl;
	}
}

void IMU::IMUManager::setAngles(float accx, float accy, float accz,
		float gyrox, float gyroy, float gyroz) {

	double raccxn, raccyn, racczn;

	float raccl = sqrt( pow(accx,2.0) + pow(accy,2.0) + pow(accz,2.0));
	raccxn = accx/raccl;
	raccyn = accy/raccl;
	racczn = accz/raccl;

	double raccxnd = acos(raccxn);
	double raccynd = acos(raccyn);
	double raccznd = acos(racczn);

	//Set initial acc values
	if(imuinit){
		rest[R_X] = raccxn;
		rest[R_Y] = raccyn;
		rest[R_Z] = racczn;

		rgyro[R_X]= gyrox;
		rgyro[R_Y]= gyroy;
		rgyro[R_Z]= gyroz;
		imuinit = false;
	}

	double axz_prev = atan2(rest[R_X],rest[R_Z]);
	double ayz_prev = atan2(rest[R_Y],rest[R_Z]);

	double axz = axz_prev + rgyro[R_X]*T;
	double ayz = ayz_prev + rgyro[R_Y]*T;

	double rxgyro = sin(axz) / sqrt(1 + pow(cos(axz),2.0) * pow(tan(ayz),2.0));
	double rygyro = sin(ayz) / sqrt(1 + pow(cos(ayz),2.0) * pow(tan(axz),2.0));
	double rzgyrotemp = sqrt(1 - pow(rgyro[R_Z],2.0) - pow(rgyro[R_Y],2.0));
	double rzgyro = rzgyrotemp>=0?rzgyrotemp:rzgyrotemp*-1;


	rest[R_X] = (accx + rxgyro * FILTER_WEIGHT ) / (1 + FILTER_WEIGHT);
	rest[R_Y] = (accy + rygyro * FILTER_WEIGHT ) / (1 + FILTER_WEIGHT);
	rest[R_Z] = (accz + rzgyro * FILTER_WEIGHT ) / (1 + FILTER_WEIGHT);

	double r = sqrt(pow(rest[R_X],2.0) + pow(rest[R_Y],2.0) +  pow(rest[R_Z],2.0));

	rest[R_X] = rest[R_X]/r;
	rest[R_Y] = rest[R_Y]/r;
	rest[R_Z] = rest[R_Z]/r;

	if(imuinitn <1000){
		imuinitn++;
	}else{
		 // set imupack
		imupack.setSensorDataValue(ROLL,rest[R_Y]);
		imupack.setSensorDataValue(PITCH,rest[R_X]);
		imupack.setSensorDataValue(YAW,rest[R_Z]);

		// Should we rumble (force feedback)
		imupack.setSensorDataValue(HEAVE,0);
		imupack.setSensorDataValue(SWAY,0);
		imupack.setSensorDataValue(SURGE,0);
	}
}

void IMU::IMUManager::filterData() {

	float accx = (imudata.accx*FILTER_RATIO)+(old.accx*(1-FILTER_RATIO));
	float accy = (imudata.accy*FILTER_RATIO)+(old.accy*(1-FILTER_RATIO));
	float accz = (imudata.accz*FILTER_RATIO)+(old.accz*(1-FILTER_RATIO));
	float gyrox= (imudata.gyrox*(1-FILTER_RATIO))+(old.gyrox*(FILTER_RATIO));
	float gyroy= (imudata.gyroy*(1-FILTER_RATIO))+(old.gyroy*(FILTER_RATIO));
	float gyroz= (imudata.gyroz*(1-FILTER_RATIO))+(old.gyroz*(FILTER_RATIO));

	imudata.accx = accx;
	imudata.accy = accy;
	imudata.accz = accz;

	//Highpass for gyroscope will compromise measurements DO NOT USE
	//imudata.gyrox= gyrox;
	//imudata.gyroy= gyroy;
	//imudata.gyroz= gyroz;

	old.accx = imudata.accx;
	old.accy = imudata.accy;
	old.accz = imudata.accz;
	old.gyrox =imudata.gyrox;
	old.gyroy =imudata.gyroy;
	old.gyroz =imudata.gyroz;
}

IMU::IMUManager::~IMUManager() {
}