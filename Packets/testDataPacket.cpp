/*
 * testDataPacket.cpp
 *
 *  Created on: Apr 15, 2014
 *      Author: Hakan Therén
 */

#include <stdio.h>
#include <inttypes.h>
#include "DataPacket.h"

int main(){
	struct header headerout;
	struct data dataout;
	struct header headerin;
	struct data datain;

	headerout.packetId = 12321123;
	dataout.pin = 5;
	dataout.value = 765756765;

	char communication[10];

	packHeaderData(headerout,dataout, communication);
	unpackHeaderData(&headerin,&datain, communication);

	printf("Sent values!\n");
	printf("packetId: %u\n", headerout.packetId);
	printf("pin: %u\n", dataout.pin);
	printf("value: %u\n", dataout.value);


	printf("\nRecieved values!\n");
	printf("packetId: %u\n", headerin.packetId);
	printf("pin: %u\n", datain.pin);
	printf("value: %u\n", datain.value);

	return 0;
}
