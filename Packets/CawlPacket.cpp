/*
 * CawlPacket.cpp
 *
 *  Created on: Apr 17, 2014
 *      Author: cawl-mobile
 */

#include "CawlPacket.h"

// Change to namespace Packets
using namespace  Packets;
CawlPacket::CawlPacket(uint8_t p ,uint8_t s,char* d) {
	data = d;
	prio = p;
	streamId = s;

}

CawlPacket::~CawlPacket() {

}

/* namespace std */
