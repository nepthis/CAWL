/*
 * CawlPacket.cpp
 *
 *  Created on: Apr 17, 2014
 *      Author: Håkan Therén
 *
 *      Description: Class to be used for storing data when sending in CawlSocket.cpp
 */

#include "CawlPacket.h"
#include <string>
#include <cstring>


using namespace  Packets;
CawlPacket::CawlPacket(uint8_t p ,uint8_t s,char* d) {
	SetData(d);

	prio = p;
	streamId = s;
	time_arr = time_snd = data_size = 0;
}

CawlPacket::~CawlPacket() {
}

void Packets::CawlPacket::SetData(char* d) {
	memset(data,'\0',sizeof(data));
	size_t i;
		for(i=0; d[i]; i++){
			data[i]=d[i];
		}

}

char* Packets::CawlPacket::GetData() {
	return data;
}
/* namespacestd */
