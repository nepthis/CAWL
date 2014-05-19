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
	data = d;
	prio = p;
	streamId = s;
}

CawlPacket::~CawlPacket() {
}

/* namespace std */
