/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
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
	DigitalOut dio;
	while(true){
		if(toggle){
			toggle = 0;
			float temp = toggle*3.0; //pretty close to max Amp for the solonoid used
			ao.setChannelValue(temp, AO_7);
			ao.setDestination(2);
			dio.setDestination(2);
			dio.setDigitalOut(DO22_EA61, 0);
			dio.setDigitalOut(DO31_EA52, 1);
			tempAn = em.recAnalogIn();
			tempIn = em.recDigitalIn();
			if((tempAn.getSource())==2 && (tempIn.getSource() == 2)){
				em.sendAnalogCommand(ao.getChannel(), 2);
				em.sendDigitalCommand(dio.getChannel(),2);
			}
		}else{
			toggle = 1;
			float temp = toggle*3.0; //pretty close to max Amp for the solonoid used
			ao.setChannelValue(temp, AO_7);
			ao.setDestination(2);
			dio.setDestination(2);
			dio.setDigitalOut(DO22_EA61, 1);
			dio.setDigitalOut(DO31_EA52, 0);
			tempAn = em.recAnalogIn();
			tempIn = em.recDigitalIn();
			if((tempAn.getSource())==2 && (tempIn.getSource() == 2)){
						em.sendAnalogCommand(ao.getChannel(), 2);
						em.sendDigitalCommand(dio.getChannel(),2);
					}
		}
	}
	printf("Done\n");
}

