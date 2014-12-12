/*
 * Ground.cpp
 *  Author: Robin Bond & H�kan Ther�n
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */

#include "Ground.h"
using namespace std;
using namespace Packets;
using namespace Ground_control;

mutex m_state;
mutex m_ImuStateToSim;

Ground::Ground(bool sctpStatus) {
	sctpIsOn = sctpStatus;
	slen = sizeof(grAddr);
	sp 				=  SimPack();
	simulator 		= new Sim();
	//	if(sctpIsOn){
	//------------------------------------SCTP------------------------------------------
	//		sockaddr_in addrSCTP = {0};
	//		sctp_sndrcvinfo sinfoSCTP = {0};
	//		sctp_event_subscribe eventSCTP = {0};
	//		pRecvBuffer[RECVBUFSIZE + 1] = {0};
	//		//MORE

	//}else{
	//-------------------------------------UDP------------------------------------------
	if ((grSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		logError(strerror(errno));
		exit(1);}
	memset((char *)&grAddr, 0, sizeof(grAddr));
	inet_pton(AF_INET, DEST_ADDR, &(grAddr.sin_addr));
	grAddr.sin_port = htons(DEST_PORT);
	logVerbose("UDP socket for remote commands is set up");
	//----------------------------------------------------------------------------------
	//	}
	if ((recImuSocket = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		logWarning("Mobile -> Mobile: recImuSocket, could not set up socket.");
		sleep(1);
		logVerbose("Mobile -> Mobile: Retrying to set up recImuSocket");
	}else{
		logVerbose("Mobile -> Mobile: Set up of recImuSocket done...");
	}
	memset((char *)&recImuAddr, 0, slen);
	if(inet_pton(AF_INET, "0.0.0.0", &(recImuAddr.sin_addr)) < 0){logError("Ground -> Constructor");logError(strerror(errno));exit(1);}
	recImuAddr.sin_port = htons(REC_IMU_PORT);
	if (bind(recImuSocket, (struct sockaddr *)&recImuAddr, sizeof(recImuAddr)) < 0){
		logError("Mobile -> Mobile: bind for recImuSocket");logError(strerror(errno));exit(1);
	}
}
/* The sendPacket method receives a packet from the simulator containing data
 * 	on how the current position on controls are. This packet is then transferred to Mobile.
 * 	the state must be fixed and a separate thread must be created for receiving packets and changing the state
 */
void Ground::sendMobile() { //NOTE: This is for UDP. Write another for sctp
	int errorMobile = 0;
	logVerbose("Ground -> sendMobile: starting");
	while(not signaled){
		usleep(1000);
		SimPack temp;
		m_state.lock();
		temp = state;
		m_state.unlock();
		if(sendto(grSocket, (char*)&temp.fs, sizeof(temp.fs), 0, (struct sockaddr*) &grAddr, slen) < 0){
			logWarning("Ground:sendMobile: ");
			errorMobile++;
			continue;
		}else{
			errorMobile = 0;
		}
		if(errorMobile == 1000){
			errno = ECOMM;
			logError(strerror(errno));
			logError("Fatal: cannot send to Mobile: ");
			exit(1);
		}

	}
}
void Ground::receiveSim(){
	int errorsSim = 0;
	logVerbose("Ground -> receiveSim: starting");
	while((errorsSim <= 50) && not signaled){
		try{
			sp = simulator->recvSim();
			errorsSim = 0;
		}
		catch(int e){
			logWarning("Ground -> receiveSim: could not receive packet from the simulator");
			errorsSim ++;
			continue;
		}
		m_state.lock();
		if(not (sp == state)){state = sp;}
		m_state.unlock();
		continue;
	}
	errno = ENETUNREACH;
	logError(strerror(errno));
	logError("Fatal: Ground -> receiveSim");
	exit(1);
}

/* This function also needs a state for the IMU data and then another function can call sim and send
 * 	the state
 */
void Ground::receiveImuPacket(){
	char buffer[255];
	int imuErrors = 0;
	Packets::ImuPack impa = Packets::ImuPack();
	logVerbose("Ground -> receiveImuPacket: starting");
	while(not signaled){
		if(imuErrors < 100){
			try{
				if(recvfrom(recImuSocket, buffer, 255, 0, (struct sockaddr *)&recImuAddr, &slen) <0 ){
					logWarning("Ground -> receiveImuPacket: could not receive imuPacket");
					imuErrors++;
					continue;
				}
				if(memcpy(&impa.sens, buffer, sizeof(impa.sens)) <0){
					logWarning("Ground -> receiveImuPacket: memcpy");
					imuErrors++;
					continue;
				}
				imuErrors = 0;
				m_ImuStateToSim.lock();
				imuStateToSim = impa;
				m_ImuStateToSim.unlock();
			}catch(int e){
				logError(strerror(errno));
				logError("Fatal: Ground -> receiveImuPacket");
				imuErrors = 100;
				continue;
			}
		}else{
			errno = ENOLINK;
			logError(strerror(errno));
			logError("Fatal: Failed to receive imuPackets for an extended period");
			delete &mp;
			sleep(5);
			exit(1);
		}
	}
}

void Ground_control::Ground::sendSim() {
	int errors = 0;
	Packets::ImuPack temp;
	while(not signaled){
		if (errors >= 100){
			errno = 70; //ECOMM
			logError(strerror(errno));
			logError("Fatal: Ground -> sendSim: too many failures on sending the data");
		}
		m_ImuStateToSim.lock();
		temp = imuStateToSim;
		m_ImuStateToSim.unlock();
		try{
			mp.sendSim(temp);
			errors = 0;
		}catch(int e){
			errors++;
			logWarning("Ground -> sendSim: Could not send IMU data to sim");
		}
		usleep(17000);//60Hz, this is what Oryx claims their code is running at max
	}

}

Ground::~Ground() {
	delete simulator;
}


