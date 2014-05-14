/*
 * CawlRcvTest.cpp
 *
 *  Created on: May 12, 2014
 *      Author: cawl-mobile
 */

#include <stdio.h>
#include <string>
#include <stdint.h>

#include "../Netapi/CawlSocket.h"
#include "../Netapi/Host.h"
//#include "../Netapi/CawlSocket.cpp"
#include "../Packets/CawlPacket.h"

using namespace std;

int main(){
	try{
	Netapi::Host h = Netapi::Host((char*)"127.0.0.1", 1235, (char*)"127.0.0.1");
	Netapi::CawlSocket ssocket = Netapi::CawlSocket(h);
	Packets::CawlPacket packet = Packets::CawlPacket((uint8_t)1,(uint8_t)1,(char*)"TESTTTTTAAAAAAAAAAAAAAAAATTTT");

	ssocket.send(packet);

	}catch (int e){
		printf("Exception, nr: %i \n", e);
		printf("After bind errno: %d\n", errno);
		perror("Description: ");
	}
	//printf("Data recieved: %s  \n", (char*)"ch");
}
