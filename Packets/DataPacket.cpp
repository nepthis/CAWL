/*
 * DataPacket.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: Hakan Theren
 *
 * Description: Functions for packing data header before sending them via socket.
 */

#include "DataPacket.h"
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>


void packHeader(header h, char buffer[4]) {
    uint32_t u32;
    u32 = htonl(h.packetId);
	memcpy(buffer, &u32, 4);
}


void packData(data d, char buffer[6]) {
    uint16_t u16;
    uint32_t u32;
    u16 = htons(d.pin);
    u32 = htonl(d.value);
	memcpy(buffer+0, &u16, 2);
    memcpy(buffer+2, &u32, 4);
}

/* Used for packing a header struct
 * to char array suitable for socket */

void packHeaderData(header h, data d, char buffer[10]) {
    packHeader(h, buffer+0);
    packData(d, buffer+4);
}



void unpackHeader(header *h, char buffer[4]) {
	uint32_t u32;
	memcpy(&u32, buffer, 4);
	h->packetId = ntohl(u32);
}


void unpackData(data *d, char buffer[6]) {
	uint16_t u16;
	uint32_t u32;
	memcpy(&u16, buffer, 2);
	memcpy(&u32, buffer+2, 4);
	d->pin = ntohs(u16);
	d->value = ntohl(u32);
}

/* Used for unpacking a char array to header struct
 * used when recieving from a socket */

void unpackHeaderData(header *h, data *d, char buffer[10]) {
    unpackHeader(h, buffer+0);
    unpackData(d, buffer+4);
}
