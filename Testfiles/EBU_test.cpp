/*
 * EBU_test.cpp
 *
 *  Created on: Sep 9, 2014
 *      Author: cawl-server
 */


#include "../EBU/EBUManager.h"
#include "../Packets/EBURelayPacket.h"
int main(){
	EBU::EBUManager em = EBU::EBUManager();
	if(em.connectToEBU()){
		printf("connection to EBU successful\n");
	}

	Packets::EBURelayPacket rPackOne = Packets::EBURelayPacket();
	rPackOne.setRelayValue(R_A9,1);  		//9 and 10 are used for
	rPackOne.setRelayValue(R_A10,1);		// boom
	rPackOne.setRelayValue(R_A11,1);		//11 and 12 are used for bucket
	rPackOne.setRelayValue(R_A12,1);
	rPackOne.setRelayValue(R_D9,1); 			//digital 9, 10 and 11 are
	rPackOne.setRelayValue(R_D10,1);		//used for gears
	em.sendRelayCommand(rPackOne,1);
	em.recAnalogIn(1);
	printf("Done\n");
}

