/*
 * QueueTest.cpp
 *
 *  Created on: May 23, 2014
 *      Author: Robin Bond
 */
#include <queue>
#include <string.h>
//#include <sys/types.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../Packets/CawlPacket.h"
#include "../Packets/EBUPacketAnalogOut.h"

int main(void){
	std::queue<Packets::CawlPacket> packetBuffer = std::queue<Packets::CawlPacket>();
	Packets::EBUPacketAnalogOut analogOne =  Packets::EBUPacketAnalogOut();
	Packets::EBUPacketAnalogOut analogTwo =  Packets::EBUPacketAnalogOut();
	//Packets::EBUPacketAnalogOut analogThree =  Packets::EBUPacketAnalogOut();
	//Packets::EBUPacketAnalogOut analogFour =  Packets::EBUPacketAnalogOut();
	char *tempbuffer;
	Packets::CawlPacket inPacket =  Packets::CawlPacket(1,1, tempbuffer);
	Packets::CawlPacket outPacket = Packets::CawlPacket(0,0, tempbuffer);
	//printf("size of analogpacket: %i\n", sizeof(analogOne));


	//----------------------------------------------------------------Sida 1-----------------------------------------------------------------------------
	analogOne.setDestination(1);
	analogOne.setChannelValue(3,AO_9);

	printf("voltage in analogPacket before packing is: %i\n", analogOne.getChannelValue(AO_9));
	printf("destination is: %i\n", analogOne.getDestination());

	char *thetemp;
	thetemp = (char*) malloc(sizeof(analogOne));
	memcpy(thetemp, &analogOne, sizeof(analogOne));
	memcpy(inPacket.data, thetemp, sizeof(analogOne));
	packetBuffer.push(inPacket);



	//----------------------------------------------------------------Sida 2-----------------------------------------------------------------------------
	outPacket = packetBuffer.front();
	packetBuffer.pop();
	char *temp2;
	temp2 = (char*) malloc(sizeof(analogTwo));
	memcpy(temp2, outPacket.data, sizeof(analogTwo));
	memcpy(&analogTwo, temp2, sizeof(analogTwo));
	printf("analogTwo contains volt: %i\n", analogTwo.getChannelValue(AO_9));
	printf("destination is: %i\n", analogTwo.getDestination());

	//outPacket.SetData(inPacket.GetData());

//	char *temp2;
//	temp2 = (char*) malloc(sizeof(analogThree)+1);

	//memcpy(temp2, outPacket.data, sizeof(analogThree));
	//memcpy(&analogThree, temp2, sizeof(analogThree));

	//printf("voltage in analogPacket after sending is: %i\n", analogThree.getChannelValue(AO_9));
	//printf("destination is: %i\n", analogThree.getDestination());

	//	printf("the size of data in inPacket: %i\n", sizeof(inPacket.data));
	//	packetBuffer.push(inPacket);
	//	printf("The buffer contains %i elements\n", packetBuffer.size());
	//	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	////	printf("preparing second packet\n");
	////	analogTwo.setDestination(2);
	////	analogTwo.setChannelValue(4, AO_9);
	////	printf("voltage in analogPacket before packing is: %i\n", analogTwo.getChannelValue(AO_9));
	////	printf("destination is: %i\n", analogTwo.getDestination());
	////	printf("%s\n",&analogTwo);
	////	memcpy(inPacket.data, &analogTwo, sizeof(analogTwo));
	////	printf("%s\n",inPacket.data);
	////	packetBuffer.push(&inPacket);
	//	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	//	printf("The buffer contains %i elements\n", packetBuffer.size());
	//	outPacket = packetBuffer.front();
	//	packetBuffer.pop();
	//	printf("the size of data in outPacket: %i\n", sizeof(outPacket.data));
	//	char *temp2;
	//	temp2 = (char*) malloc(sizeof(analogThree)+1);
	//	memcpy(temp2, outPacket.data, sizeof(analogThree));
	//	memcpy(&analogThree, temp2, sizeof(analogThree));
	//	printf("voltage in analogPacket after sending is: %i\n", analogThree.getChannelValue(AO_9));
	//	printf("destination is: %i\n", analogThree.getDestination());
	////	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	////	printf("The buffer contains %i elements\n", packetBuffer.size());
	////	outPacket = packetBuffer.front();
	////	packetBuffer.pop();
	////	memcpy(&analogFour, outPacket.data, sizeof(analogFour));
	////	printf("voltage in analogPacket after sending is: %i\n", analogFour.getChannelValue(AO_9));
	////	printf("destination is: %i\n", analogFour.getDestination());
	////	//----------------------------------------------------------------------------------------------------------------------------------------------------------
	return 1;
}





