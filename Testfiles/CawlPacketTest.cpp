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
#include <typeinfo>
#include <cstring>

using namespace::Packets;

int main(){
	CawlPacket packet = CawlPacket(1,0,(char*)"smurfarnäas vaerskstad yaoooo");
	packet.SetSnd();
	printf("Data: %s , Prio: %i , Streamid: %i , Send timestamp %ld \n",packet.data ,packet.prio ,packet.streamId ,packet.time_snd);
}
