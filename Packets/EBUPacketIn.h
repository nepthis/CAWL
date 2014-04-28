/*
 * EBUPacket.h
 *
 *  Created on: Apr 24, 2014
 *      Author: cawl-server
 */

#ifndef EBUPACKET_H_
#define EBUPACKET_H_


typedef struct EBUPackets {
	uint16_t channel[24];
};
class EBUPacketIn {
	struct EBUPackets;
public:
	EBUPacketIn();
	virtual ~EBUPacketIn();
	uint16_t getChannel();
	void setChannel(uint16_t,int pin);
};

#endif /* EBUPACKET_H_ */
