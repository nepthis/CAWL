/*
 * CawlPacket.cpp
 *
 *  Created on: Apr 17, 2014
 *      Author: Håkan Therén
 *
 *      Description: Class to be used for storing data when sending in CawlSocket.cpp
 */

#include "CawlPacket.h"

using namespace  Packets;
CawlPacket::CawlPacket(uint8_t p ,uint8_t s,char* d) {
	SetData(d);

	prio = p;
	streamId = s;
	time_arr = time_snd = data_size =time_arru = time_sndu= 0;
}

void CawlPacket::SetData(char* d) {
	memset(data,'\0',sizeof(data));
	memcpy(data,d,sizeof(data));
}

char* CawlPacket::GetData() {
	return data;
}

void CawlPacket::SetSnd() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	time_snd   = 0;
	time_sndu = 0;
	time_snd   = tp.tv_sec;
	time_sndu = tp.tv_usec;

}

void CawlPacket::SetRcv() {
	struct timeval tp;;
	gettimeofday(&tp, NULL);
	time_arr  =0;
	time_arru=0;
	time_arr  = tp.tv_sec;
	time_arru=tp.tv_usec;

}

time_t CawlPacket::GetDelay() {
	if(time_arr >= time_snd) {
//		time_t fakeTime;
//		fakeTime = 5;
//		return fakeTime;
		time_t arr_send= (time_arr - time_snd)*1000;
		time_t arru_sendu = (time_arru - time_sndu)/1000 +0.5;
		//std::cout << "getDelay: " <<arr_send << " : " << arru_sendu <<'\n';
		return (arr_send + arru_sendu);
	}else{
		return 0;
	}
}

CawlPacket::~CawlPacket() {

}

/* namespacestd */
