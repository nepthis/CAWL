/*
 * movementTest.cpp
 *
 *  Created on: Sep 9, 2014
 *      Author: Robin
 */
#include "../Simulator/Sim.h"
#include "../Packets/ImuPack.h"

using namespace Simulator;
using namespace Packets;

int main (){
	Sim s = Sim();
	ImuPack ipnew = ImuPack();
	ImuPack iprev = ImuPack();
	int retries = 5;
	while(retries){
		if(not(s.connectToSim())){
			printf("Connection to simulator failed\n");
			retries--;
		}else{
			break;
		}
	}
	//Code goes here for receiving IMU data and sending it


	return 1;
}



