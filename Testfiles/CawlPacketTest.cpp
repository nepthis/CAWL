/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
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
		CawlPacket packet = CawlPacket(1,0,(char*)"smurfarnÃ¤as vaerskstad yaoooo");
		packet.SetSnd();
		packet.SetPrio(3);
		packet.SetData((char*)"test");
		packet.SetId(5);
		usleep(2600);
		packet.SetRcv();

		printf("Data: %s , Prio: %i , Streamid: %i , Send timestamp %ld \n",packet.GetData() ,packet.GetPrio() ,packet.GetId() ,packet.GetDelay());
	}
}
