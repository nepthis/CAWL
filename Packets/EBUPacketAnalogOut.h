/*
 * EBUPacketAnalogOut.h
 *
 *  Created on: Apr 28, 2014
 *      Author: Robin Bond
 *      Desc: Container class to keep the state of the data to the EBU
 */

#ifndef EBUPACKETANALOGOUT_H_
#define EBUPACKETANALOGOUT_H_
//The defines are pin indices for the array
#define AO_1	0
#define AO_2	1
#define AO_3	2
#define AO_4	3
#define AO_5	4
#define AO_6	5
#define AO_7	6
#define AO_8	7
#define AO_9	8
#define AO_10	9
#define AO_11	10
#define AO_12	11
#define AO_13	12
#define AO_14	13
#define AO_15	14
#define AO_16	15
#define AO_17	16
#define AO_18	17
#define AO_19	18
#define AO_20	19
#define AO_21	20
#define AO_22	21
#define AO_23	22
#define AO_24	23

#include <stdint.h>

namespace Packets{
//	This struct stores the actual data for the EBU
typedef struct ebuAnOut{
	uint16_t channel;
}ebuAnOut;

/*	This class contains the struct for the pins on the EBU
 *	The methods are simple setters and getters abstracting away the array
 *	that contains the data.
 */
class EBUPacketAnalogOut {
	int destination;
	ebuAnOut toEBU;
public:
	EBUPacketAnalogOut();
	//Returns the struct, because we use classes instead of just a struct.
	ebuAnOut getChannel();
	//Returns the value at a given pin that you want to read.
	uint16_t getChannelValue(int);
	//Set the value of a specific channel/pin to the EBU. if too high, max will be set, if too low lowest will be set
	int setChannelValue(int volt, int pin);
	virtual ~EBUPacketAnalogOut();
	int getDestination(void);
	void setDestination(int dest);
};
}


#endif /* EBUPACKETANALOGOUT_H_ */
