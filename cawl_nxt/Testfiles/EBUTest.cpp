#include"../EBU/EBUManager.h"				//For communication to EBU
#include "../Packets/EBUPacketAnalogOut.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/EBURelayPacket.h"	//In order to set which relays easier, it contains all defines.

#include <stdio.h>

void main(){
	EBU::EBUManager em = EBU::EBUManager();
	Packets::EBUPacketAnalogOut 	eao 	= Packets::EBUPacketAnalogOut();
	Packets::EBURelayPacket				er		= Packets::EBURelayPacket();
	eao.setDestination(1);
	er.setRelayValue(R_A9,1);
	er.setRelayValue(R_A10,1);
	er.setRelayValue(R_A11,1);
	er.setRelayValue(R_A12,1);
	er.setRelayValue( R_A17, 1);
	em.sendRelayCommand(er, 1);
	for (int i = 1; i<= 5; i++){
		sleep(1);
		if (i == 5){
			i = 1;
		}
		eao.setChannelValue(i, AO_9);
		eao.setChannelValue(i, AO_10);
		eao.setChannelValue(i, AO_11);
		eao.setChannelValue(i, AO_12);
		eao.setChannelValue(i, AO_17);
		em.sendAnalogCommand(eao,eao.getDestination());
	}
	er.setRelayValue(R_A9,0);
	er.setRelayValue(R_A10,0);
	er.setRelayValue(R_A11,0);
	er.setRelayValue(R_A12,0);
	er.setRelayValue( R_A17, 0);
	em.sendRelayCommand(er, 1);

}
