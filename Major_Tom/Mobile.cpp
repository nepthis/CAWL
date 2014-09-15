/*
 * Mobile.cpp v0.2
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */
#include "Mobile.h"

namespace Major_Tom {
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
	stopPacket 							= Packets::EBUPacketAnalogOut();
	try{
		//q_cawlBuffer 						= std::queue<Packets::SimPack>();
		em 											= EBU::EBUManager();
//		rPackTwo.setRelayValue(R_A9,1);  		//9 and 10 are used for
//		rPackTwo.setRelayValue(R_A10,1);		// boom
//		rPackTwo.setRelayValue(R_A11,1);		//11 and 12 are used for bucket
//		rPackTwo.setRelayValue(R_A12,1);
//		rPackOne.setRelayValue(R_D9,1); 			//digital 9, 10 and 11 are
//		rPackOne.setRelayValue(R_D10,1);		//used for gears
//		rPackOne.setRelayValue(R_D11,1);
		//rPackTwo.setRelayValue(R_A17, 1);		//CDC Steering
	//	rPackTwo.setRelayValue(R_A18, 1);		//CDC Steering
		//rPackTwo.setRelayValue(R_A19, 1);		//Gas
		//rPackTwo.setRelayValue(R_A20, 1);		//Gas
		rPackTwo.setRelayValue(R_A7, 1);		//broms
	}catch(int e){
		throw e;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------

}

//This function receives UDP packets and puts them in a buffer
void Mobile::socketReceive() {
	while(not pleased){
		Packets::SimPack simpack;
		std::unique_lock<std::mutex> lockQ(m_Queue, std::defer_lock);
		try{
			char recbuf[255];
			recvfrom(mobSocket, recbuf, 255, 0, (struct sockaddr *)&mobAddr, &slen);
			memcpy(&simpack.fromSim, recbuf, sizeof(simpack.fromSim));
			lockQ.lock();
			if ((not (state == simpack))){
				state = simpack;
			}
			lockQ.unlock();
		}catch(int e){
			lockQ.unlock();
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
			em.recAnalogIn(1);
			em.recAnalogIn(2);
		}catch(int e){
			throw e;
		}
	}
}
/*	This function is responsible for receiving data from the IMU and to send it to Ground over UDP
 * 	Written by Robin
 */
void Mobile::imuSend() {
}
/*	The method ebuSend locks the packetBuffer and takes out One packet, sends it to the ebu with
 * 	the ebuManager. This method is designed to be started as a thread.
 * 	For now there is a sleep of 200000 microseconds in order to not crash the EBU.
 */
void Mobile::ebuSend() {
	while(not pleased){
		Packets::EBUPacketAnalogOut analogOne;
		Packets::EBUPacketAnalogOut analogTwo;
		Packets::EBUPacketDigitalOut digitalOne;
		Packets::EBUPacketDigitalOut digitalTwo;	//Not really used for now
		std::unique_lock<std::mutex> lock1(m_Queue, std::defer_lock);
		try{
			lock1.lock();
			setEbuOne(&state, &analogOne, &digitalOne);
			setEbuTwo(&state, &analogTwo, &digitalTwo);
			lock1.unlock();
			em.sendAnalogCommand(analogOne.getChannel(), analogOne.getDestination());
			em.sendAnalogCommand(analogTwo.getChannel(), analogTwo.getDestination());
			em.sendDigitalCommand(digitalOne.getChannel(), digitalOne.getDestination());
		}catch(int e){
			lock1.unlock();
			errno = ECOMM;
			throw 0;
		}
		usleep(200000); 		//The usleep prevents the EBU from crashing...
	}
}
void Mobile::setBoom(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(5.0-temp, AO_9);
	pkt->setChannelValue(temp, AO_10);
}
void Mobile::setBucket(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(5-temp, AO_11);
	pkt->setChannelValue(temp, AO_12);
}
void Mobile::setGas(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value*4.0+0.5;
	pkt->setChannelValue(5.0-temp, AO_19);
	pkt->setChannelValue((temp), AO_20);
}
void Mobile::setSteer(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(temp, AO_17);
	pkt->setChannelValue(5.0-temp, AO_18);
}
void Mobile::setBrake(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value*3.75;
	pkt->setChannelValue(temp, AO_7);
}
void Mobile::setGear(int p1,int  p2, int p3, Packets::EBUPacketDigitalOut* pkt){
	pkt->setDigitalOut(DO9_EA37, p1);
	pkt->setDigitalOut(DO10_EA36, p2);
	pkt->setDigitalOut(DO11_EA35, p3);

}
//Uses above functions with values from the simulator
void Mobile::setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao, Packets::EBUPacketDigitalOut* epdo) {
	epao->setDestination(1);
	epdo->setDestination(1);
	setGear(sp->getDigital(ACTIVATIONCLC), sp->getDigital(GEARCLCFORWARD), sp->getDigital(GEARCLCREVERSE), epdo);
}
//Uses above functions with values from the simulator
void Mobile::setEbuTwo(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao, Packets::EBUPacketDigitalOut* epdo) {
	epao->setDestination(2);
	epdo->setDestination(2);
	setBoom(sp->getAnalog(LIFTSTICK), epao);
	setBucket(sp->getAnalog(TILTSTICK), epao);
	setBrake(sp->getAnalog(BRAKEPEDAL), epao);
	setGas(sp->getAnalog(GASPEDAL), epao);
	setSteer(sp->getAnalog(JOYSTICK),epao);
}


Mobile::~Mobile() {
	em.sendAnalogCommand(stopPacket.getChannel(), 1);
	rPackOne = Packets::EBURelayPacket();
	rPackTwo = Packets::EBURelayPacket();
	em.sendRelayCommand(rPackOne, 1);
	em.sendRelayCommand(rPackTwo, 2);
}

} //namespace Major_Tom


