/*
 * EBU_test.cpp
 *
 *  Created on: Sep 9, 2014
 *      Author: cawl-server
 */


#include "../EBU/EBUManager.h"
#include "../Packets/RelayOut.h"
#include <stdio.h>
using namespace EBU;
using namespace Packets;
int main(){
	int toggle = 0;
	EBU::EBUManager em = EBU::EBUManager();
	if(em.connectToEBU()){
		printf("connection to EBU successful\n");
	}

	RelayOut rPackTwo;
//	rPackOne.setRelayValue(R_A9,1);  		//9 and 10 are used for
//	rPackOne.setRelayValue(R_A10,1);		// boom
//	rPackOne.setRelayValue(R_A11,1);		//11 and 12 are used for bucket
//	rPackOne.setRelayValue(R_A12,1);
//	rPackOne.setRelayValue(R_D9,1); 			//digital 9, 10 and 11 are
//	rPackOne.setRelayValue(R_D10,1);		//used for gears
	rPackTwo.setRelayValue(R_A7, 1);		//broms
	AnalogIn tempAn = em.recAnalogIn();
	DigitalIn tempIn = em.recDigitalIn();
	em.sendRelayCommand(rPackTwo, 2);
	AnalogOut ao;
	while(true){
		if(toggle){
			toggle = 0;
			float temp = toggle*3.0; //pretty close to max Amp for the solonoid used
			ao.setChannelValue(temp, AO_7);
			ao.setDestination(2);
			tempAn = em.recAnalogIn();
			tempIn = em.recDigitalIn();
			em.sendAnalogCommand(ao.getChannel(), 2);
		}else{
			toggle = 1;
			float temp = toggle*3.0; //pretty close to max Amp for the solonoid used
			ao.setChannelValue(temp, AO_7);
			ao.setDestination(2);
			tempAn = em.recAnalogIn();
			tempIn = em.recDigitalIn();
			em.sendAnalogCommand(ao.getChannel(), 2);
		}
	}
	printf("Done\n");
}

