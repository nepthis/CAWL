/*
 * CawlRcvTest.cpp
 *
 *  Created on: May 12, 2014
 *      Author: Håkan Therén
 *
 *      Description: Simple test script for testing CawlSocket.cpp
 *      			 to be used with CawlSendTest.cpp
 */

#include <stdio.h>
#include <string>
#include <stdint.h>

#include "../Netapi/CawlSocket.h"
#include "../Netapi/Host.h"
#include "../Packets/CawlPacket.h"

using namespace std;

int main(){
	try{
		Netapi::Host h = Netapi::Host((char*)"127.0.0.1", 1235, (char*)"127.0.0.1", true);
		Netapi::CawlSocket ssocket = Netapi::CawlSocket(h);
		Packets::CawlPacket packet = Packets::CawlPacket((uint8_t)1,(uint8_t)1);
		int counter = 0;
		while(1){
			counter++;
			ssocket.rec(packet);
			printf("Recieved: %s, counter is %i \n",packet.data, counter);

		}
		packet.SetData((char*)"Hmmmmmmm");
		ssocket.send(packet);
	}catch (int e){
		printf("Exception, nr: %i \n", e);
		printf("After bind errno: %d\n", errno);
		perror("Description: ");
	}
}
