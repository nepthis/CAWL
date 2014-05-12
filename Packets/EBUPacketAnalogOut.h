/*
 * EBUPacketAnalogOut.h
 *
 *  Created on: Apr 28, 2014
 *      Author: Robin Bond
 *      Desc: Container class to keep the state of the data to the EBU
 */

#ifndef EBUPACKETANALOGOUT_H_
#define EBUPACKETANALOGOUT_H_
typedef struct ebuAnOut{
	uint16_t channel[];
};
namespace Packets{
class EBUPacketAnalogOut {
	ebuAnOut toEBU;
public:
	EBUPacketAnalogOut();
	//Returns the struct, because we use classes instead of just a struct.
	ebuAnOut getChannel();
	//Returns the value at a given pin that you want to read.
	float getChannelValue(int);
	//Set the value of a specific channel/pin to the EBU. if too high, max will be set, if too low lowest will be set
	void setChannelValue(int, int);
	virtual ~EBUPacketAnalogOut();
};
}


#endif /* EBUPACKETANALOGOUT_H_ */
