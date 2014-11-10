/*
 * IMUManagere.cpp
 *
 *  Created on: Sep 8, 2014
 *      Author: cawl-mobile
 */
#include "IMUManager.h"


std::mutex data_lock;

IMU::IMUManager::IMUManager(bool imu_rec, bool sim_snd) {
	imuinit = true;
	conn = false;

	bufsize = imuinitn = 0;

	offset_accx = offset_accy = offset_accz = 0;
	old.accx = old.accy = old.accz = 0;
	old.gyrox = old.gyroy = old.gyroz = 0;

	devid = -1;

	init(imu_rec,sim_snd);
}

/*
 * Start interfacing the IMU using rs232
 */
int IMU::IMUManager::init(bool imu_rec, bool sim_snd) {
	while(devid<0){
		devid = getDev();
		if(devid == -1){sleep(5);}
	}

	if(RS232_OpenComport(devid, BAUD)) {
		return 1;
	}else{
		conn = true;
	}

	// Used for testing simulator!!
	if ((simsock = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket error");
		printf ("Error number is: %s\n",strerror(errno));
		return 1;
	}

	memset((char *)&simAddr, 0, sizeof(simAddr));
	inet_pton(AF_INET, SIM_ADDR, &(simAddr.sin_addr));
	simAddr.sin_port = htons(SIM_PORTEN);


	if(imu_rec){
		std::thread t1 (&IMU::IMUManager::readImu, this);
		std::thread t2 (&IMU::IMUManager::getControl , this);

		t1.detach();
		t2.detach();
	}
	if(sim_snd){
		std::thread t3 (&IMU::IMUManager::sendData, this);

		t3.detach();
	}

	return -1;
}

/*
 * Used to see what IMU if any is connected and to recieve it's
 * full devicename not the device path included
 * Checks dev_id vector for usable id's. Also sets stored offsets
 * for IMU
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
 * Used to get a 22 byte command from the IMU
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

			/* Trace print to get command from IMU
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
	accy = (actemp - getxoffset()) >> 2;

	memmove(&actemp,command+14,2);
	actemp = __builtin_bswap16(actemp);
	accx = (actemp - getyoffset()) >> 2;

	memmove(&actemp,command+16,2);
	actemp = __builtin_bswap16(actemp);
	accz = (actemp - getzoffset()) >> 2;

	filterData(accToFloat(accx), accToFloat(accy), accToFloat(accz),
			gyroToFloat(gyrox), gyroToFloat(gyroy*NEG_GYRO_Y), gyroToFloat(gyroz));

}



void IMU::IMUManager::filterData(double ax, double ay, double az,
		double gx, double gy, double gz) {

	double accx = (ax*FILTER_RATIO_A)+(old.accx*(1-FILTER_RATIO_A));
	double accy = (ay*FILTER_RATIO_A)+(old.accy*(1-FILTER_RATIO_A));
	double accz = (az*FILTER_RATIO_A)+(old.accz*(1-FILTER_RATIO_A));
	double gyrox= (gx*(FILTER_RATIO_G))+(old.gyrox*(1-FILTER_RATIO_G));
	double gyroy= (gy*(FILTER_RATIO_G))+(old.gyroy*(1-FILTER_RATIO_G));
	double gyroz= (gz*(FILTER_RATIO_G))+(old.gyroz*(1-FILTER_RATIO_G));

	data_lock.lock();
	imudata.accx = accx;
	imudata.accy = accy;
	imudata.accz = accz;
	imudata.gyrox= gyrox;
	imudata.gyroy= gyroy;
	imudata.gyroz= gyroz;
	data_lock.unlock();

	old.accx = accx;
	old.accy = accy;
	old.accz = accz;
	old.gyrox= gyrox;
	old.gyroy= gyroy;
	old.gyroz= gyroz;
}


/*
 * Reads data and calls filter
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

		// wait for correct data
		if(imuinitn <5){
			imuinitn++;
		}else{
			setAngles(accx,accy,accz,gyrox,gyroy,gyroz);
		}
		usleep(1000000/T);
	}
}

/*
 * Used to sensorfuse gyro and acc and get angles, and linear acc.
 * Needs to be rewritten.
 */
void IMU::IMUManager::setAngles(float accx, float accy, float accz,
		float gyrox, float gyroy, float gyroz) {

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

	double roll = acos(rest[R_X]) - M_PI / 2;
	double pitch= acos(rest[R_Y]) - M_PI / 2;

	// set imupack
	imupack.setSensorDataValue(ROLL,
			fabs(roll) > M_PI/6 ?
					copysign(M_PI/6,roll) :
					roll);

	imupack.setSensorDataValue(PITCH,
			fabs(pitch)>M_PI/6 ?
					copysign(M_PI/6,pitch) :
					pitch);

	imupack.setSensorDataValue(YAW,0);  // Use gyroz ?

	linear_accx = (raccxn - rest[R_X]);
	linear_accy = linear_accx*LIN_FILT + (raccyn - rest[R_Y])*(1-LIN_FILT);
	linear_accz = linear_accx*LIN_FILT + (racczn - rest[R_Z])*(1-LIN_FILT);

	imupack.setSensorDataValue(HEAVE,0);
	imupack.setSensorDataValue(SWAY,0);
	imupack.setSensorDataValue(SURGE,0);
	imupack.stampTime();

	//Trace: Data from IMU in deg. (acc + gyro)
	//printf("%f   %f   %f \n", (acos(rest[R_X])*57.3),(acos(rest[R_Y])*57.3),acos(rest[R_Z])*57.3);

	//Trace: Data from acc
	//printf("%f   %f   %f \n", raccxn, raccyn, racczn);

	//Trace: Data for linear acceleration
	//printf("%f   %f   %f \n", linear_accx,linear_accy,linear_accz);
	//printf("%f\n",fabs(linear_accz)>0.001?linear_accz/10:0);

	//Trace: raw gyro data
	//printf("%f   %f   %f \n", gyrox,gyroy,gyroz);

	//Trace: Compare gyro+acc / acc
	//printf("acc:,%f,%f,acc+gyro\n",(raccxnd-90),(acos(rest[R_X])*57.3)-90);

	//printf("accx:%f  accy:%f accz:%f\n",accx,accy,accz);

	//Trace: IMUPack data
	//printf("ROLL: %f PITCH: %f \n",imupack.getSensorDataValue(ROLL),imupack.getSensorDataValue(PITCH));

}

/*
 * Used to send data to Oryx platform in testing purposes
 */

void IMU::IMUManager::sendData() {
	// Wait for first data
	sleep(5);

	while(1){
		sendto(simsock, (char*)&imupack.sens, 32, 0, (struct sockaddr*) &simAddr, sizeof(struct sockaddr_in));

		// Busy wait, SIM_FREQ frequency platform recieves packages
		usleep(1000000/SIM_FREQ);
	}
}

IMU::IMUManager::~IMUManager() {
}
