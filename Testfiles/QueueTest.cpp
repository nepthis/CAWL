/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
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
	Packets::CawlPacket inPacket =  Packets::CawlPacket(1,1);
	Packets::CawlPacket outPacket = Packets::CawlPacket(0,0);



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
	exit(1);
}





