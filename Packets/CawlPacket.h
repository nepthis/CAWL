/*
 * CawlPacket.h
 *
 *  Created on: Apr 17, 2014
 *      Author: cawl-mobile
 */

#include <stdint.h>
#include <cstring>

#ifndef CAWLPACKET_H
#define CAWLPACKET_H

namespace Packets {

class CawlPacket {

public:
	uint8_t prio, streamId;
	char * data;
	CawlPacket(uint8_t p ,uint8_t s = 0, char* d =(char *)"");

	virtual ~CawlPacket();

};

} /* namespace std */

#endif /* CAWLPACKET_H_ */
