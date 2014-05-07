/*
 * CawlPacket.cpp
 *
 *  Created on: Apr 17, 2014
 *      Author: cawl-mobile
 */

#include "CawlPacket.h"

// Change to namespace Packets
namespace  Packets{

CawlPacket::CawlPacket() {
	prio = streamId = 0;
}

uint8_t CawlPacket::getPrio() {
	return prio;
}

void CawlPacket::setPrio(uint8_t i) {
	prio = i;
}

uint8_t CawlPacket::getStreamId() {
	return streamId;
}

void CawlPacket::setStreamId(uint8_t i) {
	streamId = i;
}

char CawlPacket::getData() {
	return *data;
}

void CawlPacket::setData(char c) {
	memcpy(data,&c,256);
}

CawlPacket::~CawlPacket() {
	}

} /* namespace std */
