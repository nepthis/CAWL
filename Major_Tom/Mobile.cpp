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

/*	The contructor for the Mobile gateway initializes almost everything
 * 	and also sends the relay information to the EBU enabling the needed relays.
 * 	For now sending data back over the CawlSocket is not performed but when it
 * 	is a separate socket for sending the data will be used.
 */
Mobile::Mobile() {
	//Variable setup
	//--------------------------------------------- Receiving socket from Ground-------------------------------------------------------
	slen = sizeof(mobAddr);
	if ((mobSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		throw 13;
	}
	memset((char *)&mobAddr, 0, slen);
	inet_pton(AF_INET, REC_ADDR, &(mobAddr.sin_addr));
	mobAddr.sin_port = htons(REC_PORT);
	if (bind(mobSocket, (struct sockaddr *)&mobAddr, sizeof(mobAddr)) < 0) {
		throw 14;
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------Socket for sending IMU data-----------------------------------------------------------
	//	if ((sndImuSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
	//		perror("socket error");
	//		printf ("Error number is: %s\n",strerror(errno));
	//		throw sndImuSocket; //TBD
	//	}
	//	memset((char *)&sndImuAddr, 0, sizeof(sndImuAddr));
	//	inet_pton(AF_INET, GND_ADDR, &(sndImuAddr.sin_addr));
	//	sndImuAddr.sin_port = htons(IMU_PORT);
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	pleased = false;
	try{
		//em 											= EBU::EBUManager();
		//TODO: need to find cdc gears instead of CLC
//		rPackOne.setRelayValue(R_D9,1); 			//ActivationCLC
//		rPackOne.setRelayValue(R_D10,1);		//GearCLCReverse
//		rPackOne.setRelayValue(R_D11,1);
		rPackOne.setRelayValue(R_S7, 1);		//Relay for brakelights
		rPackOne.setRelayValue(R_S4, 1);		//Relay for parking brake
		//rPackOne.setRelayValue(R_S5, 1);	//Relay for Horn
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
	}catch(int e){
		throw e;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------

}

//This function receives UDP packets and puts them in a buffer
void Mobile::socketReceive() {

	while(not pleased){
		SimPack simpack;
		char recbuf[255];
		try{
			recvfrom(mobSocket, recbuf, 255, 0, (struct sockaddr *)&mobAddr, &slen);
			memcpy(&simpack.fromSim, recbuf, sizeof(simpack.fromSim));
			m_Queue.lock();
			if ((not (state == simpack))){
				state = simpack;
			}
			m_Queue.unlock();
		}catch(int e){
			throw e;
		}
	}
}
/*	SocketSend, not in use for now, will send data back over to the Ground Gateway.
 * 	The data being sent back is not really important right now and will just be the standard AnalogIn data.
 */
void Mobile::socketSend() {
	while(not pleased){
		try{
			//em.recAnalogIn(1);
			//em.recAnalogIn(2);
		}catch(int e){
			throw e;
		}
	}
}
/*	Receives data from an IMUHandler and puts it into a state.
 * 	Written by Robin
 */
void Mobile::imuRec() {
	//TODO: Use function from håkans IMUHandler and receive IMUdata
}

/*	The method ebuSend locks the packetBuffer and takes out One packet, sends it to the ebu with
 * 	the ebuManager. This method is designed to be started as a thread.
 * 	For now there is a sleep of 200000 microseconds in order to not crash the EBU.
 * 	The data it sends comes from a state which is set in socketReceive() if it is different than the existing one
 */
void Mobile::ebuSend() {
	while(not pleased){
		AnalogOut analogOne;
		AnalogOut analogTwo;
		DigitalOut digitalOne;
		DigitalOut digitalTwo;	//Not really used for now
		try{
			m_Queue.lock(); //state is not to be used simultaneously, lock mutex
			setEbuOne(&state, &analogOne, &digitalOne);
			setEbuTwo(&state, &analogTwo, &digitalTwo);
			m_Queue.unlock();
			em.sendAnalogCommand(analogOne.getChannel(), analogOne.getDestination());
			em.sendAnalogCommand(analogTwo.getChannel(), analogTwo.getDestination());
			em.sendDigitalCommand(digitalOne.getChannel(), digitalOne.getDestination());
		}catch(int e){
			errno = ECOMM;
			throw 0;
		}
		usleep(150000); 		//The usleep prevents the EBU from crashing...needs fix
	}
}
// Set the values in the data-struct destined for the first EBU (V-ECU)
void Mobile::setEbuOne(SimPack* sp, AnalogOut* epaoOne, DigitalOut* epdoOne) {
	epaoOne->setDestination(1);
	epdoOne->setDestination(1);
	setGear(sp->getDigital(ACTIVATIONCLC), sp->getDigital(GEARCLCFORWARD), sp->getDigital(GEARCLCREVERSE), epdoOne);
	if (sp->getAnalog(BRAKEPEDAL) > 0.0){
		setBrakeLight(1, epdoOne);
	}else{
		setBrakeLight(0, epdoOne);
	}
	//setHorn(sp->getDigital(HORN), epdoOne);
}
// Set the values in the data-struct destined for the second EBU (V2-ECU)
void Mobile::setEbuTwo(SimPack* sp, AnalogOut* epaoTwo, DigitalOut* epdoTwo) {
	epaoTwo->setDestination(2);
	epdoTwo->setDestination(2);
	setBoom(sp->getAnalog(LIFTSTICK), epaoTwo);
	setBucket(sp->getAnalog(TILTSTICK), epaoTwo);
	setBrake(sp->getAnalog(BRAKEPEDAL), epaoTwo);
	setGas(sp->getAnalog(GASPEDAL), epaoTwo);
	setSteer(sp->getAnalog(JOYSTICK),epaoTwo);
	setThirdFunc(sp->getAnalog(THIRDFUNCTION), epaoTwo);
	setFourthFunc(sp->getAnalog(FOURTHFUNCTION), epaoTwo);
}
void Mobile::setBoom(float value, AnalogOut* pkt) {
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(5.0-temp, AO_9);
	pkt->setChannelValue(temp, AO_10);
}
void Mobile::setBucket(float value, AnalogOut* pkt) {
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(5-temp, AO_11);
	pkt->setChannelValue(temp, AO_12);
}
void Mobile::setGas(float value, AnalogOut* pkt) {
	float temp = value*4.0+0.5;
	pkt->setChannelValue(5.0-temp, AO_19);
	pkt->setChannelValue((temp), AO_20);
}
void Mobile::setSteer(float value, AnalogOut* pkt) {
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(temp, AO_17);
	pkt->setChannelValue(5.0-temp, AO_18);
}
void Mobile::setBrake(float value, AnalogOut* pkt) {
	float temp = value*3.0; //pretty close to max Amp for the solonoid used
	pkt->setChannelValue(temp, AO_7);
}
void Mobile::setBrakeLight(int onOff, DigitalOut *pkt){
	pkt->setDigitalOut(SO7_HB56, onOff);
}
void Mobile::setHorn(int onOff, DigitalOut *pkt){
	pkt->setDigitalOut(SO5_HB54, onOff);
}
void Mobile::setGear(int p1,int  p2, int p3, DigitalOut* pkt){
	//Kolla upp cdc växling och skicka dit istället
	pkt->setDigitalOut(DO9_EA37, p1);
	pkt->setDigitalOut(DO10_EA36, p2);
	pkt->setDigitalOut(DO11_EA35, p3);
}
void Mobile::setThirdFunc(float value, AnalogOut* pkt) {
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(5.0-temp, AO_13);
	pkt->setChannelValue(temp, AO_14);
}
void Mobile::setFourthFunc(float value, AnalogOut* pkt) {
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(temp, AO_15);
	pkt->setChannelValue(5.0-temp, AO_16);
}

Mobile::~Mobile() {
	em.sendAnalogCommand(stopPacket.getChannel(), 1);
	rPackOne = RelayOut();
	rPackTwo = RelayOut();
	em.sendRelayCommand(rPackOne, 1);
	em.sendRelayCommand(rPackTwo, 2);
}



