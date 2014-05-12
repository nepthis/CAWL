/*
 * EBUPacketAnalogIn.h
 *
 *  Created on: Apr 24, 2014
 *      Author: Robin Bond
 *      Desc: Container class to keep the state of the data from the EBU
 */

#ifndef EBUPACKETANALOGIN_H_
#define EBUPACKETANALOGin_H_


typedef struct ebuAnIn {
	uint16_t channel[];
};

namespace Packets{

class EBUPacketAnalogIn {
	ebuAnIn fromEBU;
public:
	EBUPacketAnalogIn();
	//Returns the struct, because we use classes instead of just a struct.
	ebuAnIn getChannel(void);
	//Returns the value at a given pin that you want to read.
	float getChannelValue(int);
	//Set the value of a specific channel/pin to the EBU. if too high, max will be set, if too low lowest will be set
	void setChannelValue(int, int);
	virtual ~EBUPacketAnalogIn();
};
}
#endif /* EBUPACKETANALOGIN_H_ */
