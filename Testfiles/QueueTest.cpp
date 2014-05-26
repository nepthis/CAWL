/*
 * QueueTest.cpp
 *
 *  Created on: May 23, 2014
 *      Author: Robin Bond
 */
#include <queue>
#include <string>
//#include <sys/types.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../Packets/CawlPacket.h"
#include "../Packets/EBUPacketAnalogOut.h"

int main(void){
	std::queue<Packets::CawlPacket*> packetBuffer = std::queue<Packets::CawlPacket*>();
	Packets::EBUPacketAnalogOut analogOne =  Packets::EBUPacketAnalogOut();
	Packets::EBUPacketAnalogOut analogTwo =  Packets::EBUPacketAnalogOut();
	Packets::EBUPacketAnalogOut analogThree =  Packets::EBUPacketAnalogOut();
	Packets::EBUPacketAnalogOut analogFour =  Packets::EBUPacketAnalogOut();
	Packets::CawlPacket inPacket =  Packets::CawlPacket((uint8_t)0,(uint8_t)0,(char*)"");
	Packets::CawlPacket* outPacket;// =  Packets::CawlPacket((uint8_t)0,(uint8_t)0,(char*)"");


	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	analogOne.setDestination(1);
	analogOne.setChannelValue(5,AO_9);
	printf("voltage in analogPacket before packing is: %i\n", analogOne.getChannelValue(AO_9));
	printf("destination is: %i\n", analogOne.getDestination());
	char temp[sizeof(analogOne)];
	//printf("%s\n",&analogOne);
	char packet[sizeof(analogOne)];
	memcpy(&packet, &analogOne, sizeof(analogOne));
	//printf("%s\n",inPacket.data);
	inPacket.data = packet;
	packetBuffer.push(&inPacket);
	printf("The buffer contains %i elements\n", packetBuffer.size());
	//----------------------------------------------------------------------------------------------------------------------------------------------------------
//	printf("preparing second packet\n");
//	analogTwo.setDestination(2);
//	analogTwo.setChannelValue(4, AO_9);
//	printf("voltage in analogPacket before packing is: %i\n", analogTwo.getChannelValue(AO_9));
//	printf("destination is: %i\n", analogTwo.getDestination());
//	printf("%s\n",&analogTwo);
//	memcpy(inPacket.data, &analogTwo, sizeof(analogTwo));
//	printf("%s\n",inPacket.data);
//	packetBuffer.push(&inPacket);
	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	printf("The buffer contains %i elements\n", packetBuffer.size());
	outPacket = packetBuffer.front();
	packetBuffer.pop();
	//printf("%s\n",inPacket.data);
	&packet = outPacket->data
	memcpy(&packet, outPacket->data, sizeof(analogThree));
	printf("voltage in analogPacket after sending is: %i\n", analogThree.getChannelValue(AO_9));
	printf("destination is: %i\n", analogThree.getDestination());
//	//----------------------------------------------------------------------------------------------------------------------------------------------------------
//	printf("The buffer contains %i elements\n", packetBuffer.size());
//	outPacket = packetBuffer.front();
//	packetBuffer.pop();
//	memcpy(&analogFour, outPacket.data, sizeof(analogFour));
//	printf("voltage in analogPacket after sending is: %i\n", analogFour.getChannelValue(AO_9));
//	printf("destination is: %i\n", analogFour.getDestination());
//	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	return 1;
}





