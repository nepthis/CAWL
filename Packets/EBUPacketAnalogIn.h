/*
 * EBUPacketAnalogIn.h
 *
 *  Created on: Apr 24, 2014
 *      Author: cawl-server
 */

#ifndef EBUPACKETANALOGIN_H_
#define EBUPACKETANALOGin_H_


typedef struct dataToSend {
	uint16_t channel[24];
};

namespace Packets{
class EBUPacketAnalogIn {
	struct dataToSend;
public:
	EBUPacketAnalogIn();
	virtual ~EBUPacketAnalogIn();
	dataToSend getDataToSend(void);
	void setChannel(int pin, float value);
	float getChannel(int pin);
};
}
#endif /* EBUPACKETANALOGIN_H_ */
