/*
 * CawlPacketTest.cpp
 *
 *  Created on: May 6, 2014
 *      Author: Håkan Therén
 *
 *      Description: Simple test script for testing CawlPacket.cpp
 *
 */


#include "../Packets/CawlPacket.h"
#include <stdio.h>

using namespace::Packets;

int main(){
	CawlPacket packet = CawlPacket(1,0,(char*)"smurfar");

	printf("Data: %s , Prio: %i , Streamid: %i \n",packet.data  ,packet.prio ,packet.streamId);
}
