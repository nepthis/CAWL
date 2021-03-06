/*
 * Mobile.cpp v0.2
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */
#include "Mobile.h"
using namespace  Major_Tom;
using namespace Packets;
using namespace std;
mutex m_State;
mutex m_Sendstate;
/*	The contructor for the Mobile gateway initializes almost everything
 * 	and also sends the relay information to the EBU enabling the needed relays.
 * 	For now sending data back over the CawlSocket is not performed but when it
 * 	is a separate socket for sending the data will be used.
 */
Mobile::Mobile(bool sctp) {
	sctpIsOn = sctp;
	errors = 0;
	et = EBU::EBUTranslator();
	slen = sizeof(mobAddr);
	//--------------------------------------------- Receiving socket from Ground-------------------------------------------------------
	//if (sctpIsOn){

	//}else{
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
	}
	memset((char *)&mobAddr, 0, slen);
	if(inet_pton(AF_INET, REC_ADDR, &(mobAddr.sin_addr)) < 0){perror("Mobile:Constructor");logError(strerror(errno));throw 13;}
	mobAddr.sin_port = htons(REC_PORT);
	if (bind(mobSocket, (struct sockaddr *)&mobAddr, sizeof(mobAddr)) < 0){
		logError("Mobile -> Mobile: bind for mobSocket");logError(strerror(errno));exit(1);}
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = 100000;
	if (setsockopt(mobSocket, SOL_SOCKET, SO_RCVTIMEO,&tv,sizeof(tv)) < 0){
		logError(strerror(errno));logError("Mobile -> Mobile: mobSocket options");exit(1);}
	//}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------Socket for sending IMU data-----------------------------------------------------------
	/*for (int imu = 0; imu < RETRIES; imu++){
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
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	*/
	pleased = false;

}
bool Mobile::startUp(){
	
	bool check = true;
	for (int i = 0; i< 14; i++){
		//printf("value in relaypack one: %i\n", rPackOne.er.channel[i]);
		rPackOne.er.channel[i] = 0;
		rPackTwo.er.channel[i] = 0;
		//printf("value in relaypack two: %i\n", rPackTwo.er.channel[i]);
	}
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
	rPackTwo.setRelayValue(R_A19, 1);		//Gas
	rPackTwo.setRelayValue(R_A20, 1);		//Gas
	rPackTwo.setRelayValue(R_A7, 1);		//broms
	rPackTwo.setRelayValue(R_D22,1);		//Gear_Reverse
	rPackTwo.setRelayValue(R_D31,1);		//Gear_Forward
	rPackTwo.setRelayValue(R_D12,1);		//CDC_Activation
	//--------------------------------------------------------------------------------------------------------------------------------------------------------

	logVerbose("Trying to sync up for the relay packets");

	try{
		em.recvAnalogEBUOne();
		printf("Received from Analog EBU1\n");
		em.recvDigitalEBUOne();
		printf("Received from Digital EBU1\n");
		em.sendRelayCommand(rPackOne, 1);
		logVerbose("Relay packets sent to EBU 1");
		/*em.recvAnalogEBUTwo();
		printf("Received from Analog EBU2");
		em.recvDigitalEBUTwo();
		printf("Received from Digital EBU2");
		em.sendRelayCommand(rPackTwo, 2);
		logVerbose("Relay packets sent to EBU 2");*/
	}catch(int e){
		//perror("Error with sending relay commands");
		logWarning("Mobile - > startUp: Error while syncing up EBUs for relay packets");
		check = false;
	}

	return check;
}

//This function receives UDP packets from Ground and puts them in a state if they are changed
//If enough errors are detected
void Mobile::recvGround() {
	//printf("rcvGrnd\n");
	while(not pleased){
		if(errors == 10){
			sendAllStop();
			errno = ENETDOWN;
			logError("Fatal: Mobile -> recvGround");
			logError(strerror(errno));
			sleep(1);
			exit(1);
		}
		SimPack simpack;
		char recbuf[255];
		//if(sctpIsOn){
		//DO SCTP STUFF
		//}else{
		
		int rcvd;
		if(rcvd = recvfrom(mobSocket, recbuf, 255, 0, (struct sockaddr *)&mobAddr, &slen) < 0){
			logWarning("No data received from Ground");
			errors++;
			continue;
		}else{
			errors = 0;
			//printf("rcvd ");
		}
		memcpy(&simpack.fs, recbuf, sizeof(simpack.fs));
		//}
		m_State.lock();//(not (state == simpack)) && (state.fs.timeStamp < simpack.fs.timeStamp)
		if ((not (state == simpack)&& (state.fs.packetId < simpack.fs.packetId)) && (simpack.fs.packetSize == state.fs.packetSize)){
			state = simpack;
		}else{
			errors ++;
		}
		m_State.unlock();


	}
}
/*	Receives data from an IMUHandler and puts it into a state.
 */
void Mobile::recvIMU() {
	IMU::IMUManager imm = IMU::IMUManager();
	imm.init(true, false);
	ImuPack imp;
	while(not pleased){
		usleep(1000);
		imp = imm.getImuPack();
		logVerbose("IMUdata received");
		if(sendto(sndImuSocket, (char*)&imp.sens, sizeof(imp.sens), 0, (struct sockaddr*) &sndImuAddr, slen) < 0){
			perror("Ground:sendMobile");
			logError(strerror(errno));
			exit(0);
		}
	}
}
/*	The method ebuSend locks the packetBuffer and takes out One packet, sends it to the ebu with
 * 	the ebuManager. This method is designed to be started as a thread.
 * 	The data it sends comes from a state which is set in socketReceive() if it is different than the existing one
 */
void Mobile::sendEBUOne() {
	printf("EBU1\n");
	AnalogOut analogOne;
	DigitalOut digitalOne;
	DigitalIn digitaldummy;
	AnalogIn analogdummy;
	SimPack tempState; //Locking over methods in other objects might cause problem, this is safer.
	while(not pleased){
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
			perror("sendEBUOne error");
			logError(strerror(errno));
			throw e;
			//HERE if it fails somehow and cannot send to the EBUs it should tell the watchdog to stop all
			//operations
		}

	}
}
void Mobile::sendEBUTwo() {
	printf("EBU2\n");
	AnalogOut analogTwo;
	DigitalOut digitalTwo;	//Not really used for now
	DigitalIn digitaldummy;
	AnalogIn analogdummy;
	SimPack tempState; //Locking over methods in other objects might cause problem, this is safer.
	logVerbose("Mobile -> sendEBUTro: starting.");
	while(not pleased){
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
			logError(strerror(errno));
			logError("Fatal: Mobile -> sendEBUTwo: could not send any data to EBU 2.");
			exit(1);
			//HERE if it fails somehow and cannot send to the EBUs it should tell the watchdog to stop all
			//operations
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

void Major_Tom::Mobile::recvEBUOne() {

}

void Major_Tom::Mobile::recvEBUTwo() {
}

void Major_Tom::Mobile::setSCTP() {
	sctpIsOn = true;
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

}
