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
#include <unistd.h> //for sleep
#include <typeinfo>
#include <cstring>
#include <iostream>

using namespace::Packets;

int main(){
	while(1){
		CawlPacket packet = CawlPacket(1,0,(char*)"smurfarnäas vaerskstad yaoooo");
		packet.SetSnd();
		packet.SetPrio(3);
		packet.SetData((char*)"test");
		packet.SetId(5);
		usleep(2600);
		packet.SetRcv();

		printf("Data: %s , Prio: %i , Streamid: %i , Send timestamp %ld \n",packet.GetData() ,packet.GetPrio() ,packet.GetId() ,packet.GetDelay());
	}
}
