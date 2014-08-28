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
	//-------------------------------------------------------------------------------------------------------------------------------------------------------
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
	pleased = false;
	stopPacket 							= Packets::EBUPacketAnalogOut();
	rPackOne									= Packets::EBURelayPacket();
	rPackTwo									= Packets::EBURelayPacket();
	try{
		q_cawlBuffer 						= std::queue<Packets::SimPack>();
		state = Packets::SimPack();
		em 											= EBU::EBUManager();
		rPackOne.setRelayValue(R_A9,1);  		//9 and 10 are used for
		rPackOne.setRelayValue(R_A10,1);		// boom
		rPackOne.setRelayValue(R_A11,1);		//11 and 12 are used for bucket
		rPackOne.setRelayValue(R_A12,1);
		rPackOne.setRelayValue(R_D9,1); 			//digital 9, 10 and 11 are
		rPackOne.setRelayValue(R_D10,1);		//used for gears
		rPackTwo.setRelayValue(R_A17, 1);		//CDC Steering
		rPackTwo.setRelayValue(R_A18, 1);		//CDC Steering
		em.sendRelayCommand(rPackOne, 1);
		em.sendRelayCommand(rPackTwo, 2);
	}catch(int e){
		throw e;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------------------

}

//This function receives UDP packets and puts them in a buffer
void Mobile::socketReceive() {
	while(not pleased){
		usleep(200000);
		std::unique_lock<std::mutex> lockQ(m_Queue, std::defer_lock);
		try{
			char recbuf[255];
			Packets::SimPack simpack = Packets::SimPack();
			recvfrom(mobSocket, recbuf, 255, 0, (struct sockaddr *)&mobAddr, &slen);
			memcpy(&simpack.fromSim, recbuf, sizeof(simpack.fromSim));
			printf("Packet received has value %f\n", simpack.getAnalog(BRAKEPEDAL));
			lockQ.lock();
			if ((not (state == simpack))){
				state = simpack;
				printf("state updated\n");
			}
			lockQ.unlock();
		}catch(int e){
			throw e;
		}
	}
	printf("socketRec done\n");
}
/*	SocketSend, not in use for now, will send data back over the CawlSocket to the Ground Gateway.
 * 	Data that should be sent back could be data for the video stream or audio feedback etc
 */
void Mobile::socketSend() {
	while(not pleased){
		try{
			//To the other gateway running "ground"
		}catch(int e){
			throw e;
		}
	}

}
/*	The method ebuSend locks the packetBuffer and takes out One packet, sends it to the ebu with
 * 	the ebuManager. This method is designed to be started as a thread.
 * 	For now there is a sleep of 200000 microseconds in order to not crash the EBU.
 */
void Mobile::ebuSend() {
	while(not pleased){
		std::unique_lock<std::mutex> lock1(m_Queue, std::defer_lock);
		lock1.lock();
		Packets::EBUPacketAnalogOut analogOne;
		Packets::EBUPacketAnalogOut analogTwo;
		Packets::EBUPacketDigitalOut digitalOne;
		Packets::EBUPacketDigitalOut digitalTwo;	//Not really used for now
		try{;
			setEbuOne(&state, &analogOne, &digitalOne);
			setEbuTwo(&state, &analogTwo, &digitalTwo);
			em.sendAnalogCommand(analogOne.getChannel(), analogOne.getDestination());
			em.sendAnalogCommand(analogTwo.getChannel(), analogTwo.getDestination());
			em.sendDigitalCommand(digitalOne.getChannel(), digitalOne.getDestination());
			lock1.unlock();
		}catch(int e){
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
	pkt->setChannelValue((value*5.0), AO_19);
	pkt->setChannelValue((value*5.0), AO_20);
}
void Mobile::setSteer(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(5.0-temp, AO_17);
	pkt->setChannelValue(temp, AO_18);
}
void Mobile::setBrake(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value*4+0.5;
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
	setBoom(sp->getAnalog(LIFTSTICK), epao);
	setBucket(sp->getAnalog(TILTSTICK), epao);
	setGear(sp->getDigital(ACTIVATIONCLC), sp->getDigital(GEARCLCFORWARD), sp->getDigital(GEARCLCREVERSE), epdo);
}
//Uses above functions with values from the simulator
void Mobile::setEbuTwo(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao, Packets::EBUPacketDigitalOut* epdo) {
	epao->setDestination(2);
	epdo->setDestination(2);
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

