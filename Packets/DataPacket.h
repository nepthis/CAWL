/*
 * DataPacket.h
 *
 * Placeholder of data communicated between nodes
 *
 *  Created on: Apr 15, 2014
 *      Author: Hakan Theren
 */


#include <stdint.h>

#ifndef DATAPACKET_H_INCLUDED
#define DATAPACKET_H_INCLUDED

struct header {
	uint32_t packetId;
};

struct data {
	uint16_t pin;
	uint32_t value;
};

void packHeader(header h, char buffer[4]);
void packData(data d, char buffer[6]);
void packHeaderData(header h, data d, char buffer[10]);

void unpackHeader(header *h, char buffer[4]);
void unpackData(data *d, char buffer[6]);
void unpackHeaderData(header *h, data *d, char buffer[10]);

#endif
