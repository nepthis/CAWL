/*
 * Mobile.cpp
 *  Created on: Jun 23, 2014
 *  Author: Robin Bond & H�kan Ther�n
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */
#include "Mobile.h"
using namespace  Major_Tom;
using namespace Packets;
using namespace std;
mutex m_State;
mutex m_ImuState;
/*	The constructor for the Mobile gateway initializes almost everything
 * 	and also sends the relay information to the EBU enabling the needed relays.
 * 	For now sending data back over the CawlSocket is not performed but when it
 * 	is a separate socket for sending the data will be used.
 */
Mobile::Mobile() {
	errors = 0;
	et = EBU::EBUTranslator();
	slen = sizeof(mobAddr);
	//--------------------- Receiving socket from Ground-------------------------------------------------
	for (int gnd = 0; gnd < RETRIES; gnd++){
		if ((mobSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
			logWarning("Mobile -> Mobile: mobSocket, could not set up socket.");
			sleep(1);
			logVerbose("Mobile -> Mobile: Retrying to set up mobSocket");
			continue;
		}else{
			logVerbose("Mobile -> Mobile: Set up of mobSocket done...");
			break;
		}
		logError(strerror(errno));
		logError("Fatal: Mobile -> Mobile: Could not set up the mobile socket");
		exit(1);
	}
	memset((char *)&mobAddr, 0, slen);
	if(inet_pton(AF_INET, REC_ADDR, &(mobAddr.sin_addr)) < 0){perror("Mobile:Constructor");logError(strerror(errno));throw 13;}
	mobAddr.sin_port = htons(REC_PORT);
	if (bind(mobSocket, (struct sockaddr *)&mobAddr, sizeof(mobAddr)) < 0){
		logError(strerror(errno));logError("Fatal: Mobile -> Mobile: bind for mobSocket");exit(1);}
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	if (setsockopt(mobSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0){
		logError(strerror(errno));logError("Fatal: Mobile -> Mobile: mobSocket options");exit(1);}
	//------------------------------------------------------------------------------------------------------
	//--------------------------Socket for sending IMU data-------------------------------------------------
	for (int imu = 0; imu < RETRIES; imu++){
		if ((sndImuSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
			logWarning("Mobile -> Mobile: sndImuSocket, could not set up socket.");
			continue;
		}else{
			break;
		}
		logError(strerror(errno));
		logError("Fatal: Mobile -> Mobile: Could not set up the IMU socket");
	}
	memset((char *)&sndImuAddr, 0, sizeof(sndImuAddr));
	inet_pton(AF_INET, DESTI_ADDR, &(sndImuAddr.sin_addr));
	sndImuAddr.sin_port = htons(IMU_PORT);
	//-------------------------------------------------------------------------------------------------------


}

bool Mobile::startUp(){
	bool check = true;
	for (int i = 0; i< 14; i++){
		rPackOne.er.channel[i] = 0;
		rPackTwo.er.channel[i] = 0;
	}//for some reason initialising the arrays to 0 did not work, ugly fix.
	//-----------------------------Packet setup--------------------------------------------------
	rPackOne.setRelayValue(R_S7, 1);		//Relay for brakelights
	//rPackOne.setRelayValue(R_S4, 1);		//Relay for parking brake
	//rPackOne.setRelayValue(R_S5, 1);	//Relay for Horn, not workin...maybe.
	rPackTwo.setRelayValue(R_A9,1);  		//Lift/sink 1st
	rPackTwo.setRelayValue(R_A10,1);		// lift/sink 2nd
	rPackTwo.setRelayValue(R_A11,1);		//tilt 1st
	rPackTwo.setRelayValue(R_A12,1);		//tilt 2nd
	rPackTwo.setRelayValue(R_A13, 1);		//Third Function 1st
	rPackTwo.setRelayValue(R_A14, 1);		//Third Function 2nd
	rPackTwo.setRelayValue(R_A15, 1);		//Fourth Function 2nd
	rPackTwo.setRelayValue(R_A16, 1);		//Fourth Function 1st
	rPackTwo.setRelayValue(R_A17, 1);		//CDC Steering
	rPackTwo.setRelayValue(R_A18, 1);		//CDC Steering
	rPackTwo.setRelayValue(R_A19, 1);		//Gaspedal
	rPackTwo.setRelayValue(R_A20, 1);		//Gaspedal
	rPackTwo.setRelayValue(R_A7, 1);		//brake
	rPackTwo.setRelayValue(R_D22,1);		//Gear_Reverse
	rPackTwo.setRelayValue(R_D31,1);		//Gear_Forward
	rPackTwo.setRelayValue(R_D12,1);		//CDC_Activation
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	logVerbose("Trying to sync up for the relay packets");
	try{
		em.recvAnalogEBUOne();
		em.recvDigitalEBUOne();
		em.sendRelayCommand(rPackOne, 1);
		logVerbose("Relay packets sent to EBU 1");
		em.recvAnalogEBUTwo();
		em.recvDigitalEBUTwo();
		em.sendRelayCommand(rPackTwo, 2);
		logVerbose("Relay packets sent to EBU 2");
	}catch(int e){
		logWarning("Mobile - > startUp: Error while syncing up EBUs for relay packets");
		check = false;
	}
	//-----------------------------Serial port for Watchdog--------------------------------------------------
	logVerbose("Opening the comport for the watchdog");
	for(int i = 0; i < RETRIES; i++){
		if(RS232_OpenComport(16,9600)){
			errno = ECANCELED;
			logError(strerror(errno));
			logError("Mobile: Could not connect to watchdog");
			logVerbose("Mobile: Be sure to start application as root. Or make application member of dialout.");
			check = false;
			continue;
		}else{
			check = true;
		}
	}

	return check;

}

//This function receives UDP packets from Ground and puts them in a state if they are changed
//If enough errors are detected
void Mobile::recvGround() {
	while(not signaled){
		if(errors >= 15){
			sendAllStop();
			errno = ENETDOWN;
			logError("Fatal: Mobile -> recvGround");
			logError(strerror(errno));
			usleep(100000);
			exit(1);
		}
		SimPack simpack;
		char recbuf[255];
		if(recvfrom(mobSocket, recbuf, 255, 0, (struct sockaddr *)&mobAddr, &slen) < 0){
			logWarning("No data received from Ground");
			errors++;
			continue;
		}else{
			errors = 0;
		}
		memcpy(&simpack.fs, recbuf, sizeof(simpack.fs));
		m_State.lock();//(not (state == simpack)) && (state.fs.timeStamp < simpack.fs.timeStamp)
		if ((not (state == simpack)&& (state.fs.packetId < simpack.fs.packetId)) && (simpack.fs.packetSize == state.fs.packetSize)){
			state = simpack;
		}else{
			errors ++;
		}
		m_State.unlock();
	}
}
/*	Sends IMU data to Ground
 */
void Mobile::sendIMU(){
	ImuPack tempImu;
	int errors = 0;
	while (not signaled){
		m_ImuState.lock();
		tempImu = imuState;
		m_ImuState.unlock();
		if(sendto(sndImuSocket, (char*)&tempImu.sens, sizeof(tempImu.sens), 0, (struct sockaddr*) &sndImuAddr, slen) < 0){
			logError(strerror(errno));
			logError("Mobile->sendImu");
			errors++;
		}
	}
}
/*Receives data from the IMU
 */
void Mobile::recvFromIMU() {
	ImuPack imp;
	int errors = 0;
	while(not signaled){
		if (errors >= 100)
			usleep(1000);
		try{
			imp = imm.getImuPack(); //handle error for this one.
			m_ImuState.lock();
			imuState = imp;
			m_ImuState.unlock();
		}catch(int e){
			errors++;
			logError(strerror(errno));
		}


	}
}
/* The method sendEBU locks the state and copies it. The copied state is then sent with the
 * EBUManager to either EBU One or two (depends on which function, see below).
 */
void Mobile::sendEBUOne() {
	AnalogOut analogOne;
	DigitalOut digitalOne;
	DigitalIn digitaldummy;
	AnalogIn analogdummy;
	SimPack tempState; //Locking over methods in other objects might cause problem, this is safer.
	logVerbose("Mobile -> sendEBUOne: starting.");
	while(not signaled){
		m_State.lock();
		tempState = state;
		m_State.unlock();
		try{
			et.setEbuOne(&tempState, &analogOne, &digitalOne);	//Use EBUTranslator (et) to translate simdata
			digitaldummy = em.recvDigitalEBUOne();
			analogdummy = em.recvAnalogEBUOne();
			em.sendDigitalCommand(digitalOne.getChannel(), digitalOne.getDestination());
			em.sendAnalogCommand(analogOne.getChannel(), analogOne.getDestination());
		}catch(int e){
			logWarning("Mobile -> sendEBUOne: could not send any data to EBU 1.");
		}
	}
}
void Mobile::sendEBUTwo() {
	AnalogOut analogTwo;
	DigitalOut digitalTwo;	//Not really used for now
	DigitalIn digitaldummy;
	AnalogIn analogdummy;
	SimPack tempState; //Locking over methods in other objects might cause problem, this is safer.
	logVerbose("Mobile -> sendEBUTwo: starting.");
	while(not signaled){
		m_State.lock();
		tempState = state;
		m_State.unlock();
		try{
			et.setEbuTwo(&tempState, &analogTwo, &digitalTwo);
			digitaldummy = em.recvDigitalEBUTwo();
			analogdummy = em.recvAnalogEBUTwo();
			em.sendDigitalCommand(digitalTwo.getChannel(), digitalTwo.getDestination());
			em.sendAnalogCommand(analogTwo.getChannel(), analogTwo.getDestination());
		}catch(int e){
			logWarning("Mobile -> sendEBUTwo: could not send any data to EBU 2.");
			//change condition that watchdog looks for.
		}
	}
}

Mobile::~Mobile() {
	//	em.sendAnalogCommand(stopPacket.getChannel(), 1);
	//	rPackOne = RelayOut();
	//	rPackTwo = RelayOut();
	//sendAllStop();
	//em.sendRelayCommand(rPackOne, 1);
	//em.sendRelayCommand(rPackTwo, 2);
}
/* Based on multiple conditions found in globals.h this should constantly send
 * the char 'P' to the watchdog.
 * Baurdrate 9600, 8 bits, 1 stopbit and no parity. TTYUSB0
 */
void Major_Tom::Mobile::watchDog() {
	while(not signaled){
		if(RS232_SendByte(16, 'P')){
			errno = ECOMM;
			logError(strerror(errno));
			logError("Fatal: Mobile: Could not send signal to watchdog");
			exit(1);
		}
	}

}
/* Should simply read the state that is set from the sending threads.
 * Unless the EBUs are fixed and sending can be done separately from reading.
 */
void Major_Tom::Mobile::recvEBUOne() {

}

void Major_Tom::Mobile::recvEBUTwo() {
}


void Major_Tom::Mobile::sendAllStop() {
	logWarning("Mobile -> sendAllstop: changing state for full stop.");
	Packets::SimPack stop;
	stop.setAnalog(BRAKEPEDAL, 3.0);
	stop.setAnalog(LIFTSTICK, 0.0);
	stop.setAnalog(TILTSTICK, 0.0);
	stop.setDigital(GEARCLCREVERSE, 0);
	stop.setDigital(GEARCLCFORWARD, 0);
	stop.setAnalog(JOYSTICK, 0.0);
	m_State.lock();
	state = stop;
	m_State.unlock();
	sleep(1);
	signaled = 1;
}


