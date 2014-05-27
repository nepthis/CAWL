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
	time_arr = time_snd = data_size = 0;
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
	time_snd = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

void CawlPacket::SetRcv() {
	struct timeval tp;
	gettimeofday(&tp, NULL);
	time_arr = tp.tv_sec * 1000 + tp.tv_usec / 1000;
}

time_t CawlPacket::GetDelay() {
	if(time_arr >= time_snd) {
		return time_arr - time_snd;
	}else{
		return 0;
	}
}

CawlPacket::~CawlPacket() {

}

/* namespacestd */
