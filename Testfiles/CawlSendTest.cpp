/*
 * CawlRcvTest.cpp
 *
 *  Created on: May 12, 2014
 *      Author: Håkan Therén
 *
 *      Description: Simple test script for testing CawlSocket.cpp
 *      			 to be used with CawlRcvTest.cpp
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
		Netapi::Host* h = new Netapi::Host((char*)"127.0.0.1", 1235, (char*)"127.0.0.1", false);
		Netapi::CawlSocket* ssocket = new Netapi::CawlSocket(*h);
		Packets::CawlPacket* packet = new Packets::CawlPacket((uint8_t)1,(uint8_t)1,(char*)"TESTTTTTAAAAAAAAAAAAAAAAATTTT");
		packet->SetData((char*)"YEAHHHHHHHHHH");

		ssocket->send(*packet);

		ssocket->rec(*packet);
		delete h;
		delete ssocket;
		delete packet;
		printf("Recieved: %s , Timestamp: Send/Rcv: %ld / %ld\n",packet->GetData(),packet->GetSnd(),packet->GetRcv());
	}catch (int e){
		printf("Exception, nr: %i \n", e);
		printf("After bind errno: %d\n", errno);
		perror("Description: ");
	}
}
