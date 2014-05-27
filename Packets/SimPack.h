/*
 * SimPack.h
 *
 *  Created on: May 20, 2014
 *      Author: cawl-server
 */

#ifndef SIMPACK_H_
#define SIMPACK_H_
#include <stdint.h>
#include <time.h>


namespace Packets {
typedef struct commandPacket {
	uint32_t packetId;
	uint32_t packetSize;
	float analog[9];
	uint32_t digital; //	<-- the integers decimal value represents a binary array. to read simply convert into binary representation, 24 bits
}commandPacket;
class SimPack {
public:
	commandPacket fromSim;
	SimPack();
	void setID(uint32_t id);
	uint32_t getPacketSize(void);
	commandPacket getData(void);
	virtual ~SimPack();
};

} /* namespace Packets */

#endif /* SIMPACK_H_ */
