/* Major_Tom.cpp
 * Created on: May 16, 2014
 * Author: Robin Bond
 *	Description: The main Function of Major_Tom
 *	is to act as the application on the wheel loader's laptop that will
 *	interpret data from the EBU and send data from the simulator to the EBU.
 */

#include "Major_Tom.h"

//Används för ctrl+c, stänger av allt på ett bättre sätt. hoppas jag.
void INT_handler(int dummy){
	timeToQuit = 1;
	//exit(EXIT_SUCCESS);
}
/*
 * Resets all relays to 0
 */
void resetRelays(void){
	rPack = Packets::EBURelayPacket();
	ebuMan.sendRelayCommand(rPack, 1);
	//ebuMan.sendRelayCommand(rPack, 2);
}
void stopTheMachine(void){
	rPack = Packets::EBURelayPacket();
	ebuMan.sendAnalogCommand(stopPacket, 1);
}
//Ugly way of comparing packages, for now.
bool compPack(Packets::EBUPacketAnalogOut p1){
	bool out = false;
	if(p1.getChannelValue(AO_9) == buff.getChannelValue(AO_9)){
		out = true;
	}else{
		out = false;
	}
	if(p1.getChannelValue(AO_10) == buff.getChannelValue(AO_10)){
		out = true;
	}else{
		out = false;
	}
	if(p1.getChannelValue(AO_11) == buff.getChannelValue(AO_11)){
		out = true;
	}else{
		out = false;
	}
	if(p1.getChannelValue(AO_12) == buff.getChannelValue(AO_12)){
		out = true;
	}else{
		out = false;
	}
	return out;
}
void recPacket(){
	Packets::CawlPacket recPack =  Packets::CawlPacket(0);
	Packets::EBUPacketAnalogOut analogOut =  Packets::EBUPacketAnalogOut();
	while(not timeToQuit){
		if(m_cs.try_lock()){
			try{
				gatewaySocket.rec(recPack);
				m_cs.unlock();
			}catch(int e){
				printf("ERROR %i\n", e);
				resetRelays();
				m_cs.unlock();
				perror("Nope");
			}

			m_pb.lock();
			char *tempbuff;
			tempbuff = (char*) malloc(sizeof(analogOut));
			memcpy(tempbuff, recPack.data, sizeof(analogOut));
			memcpy(&analogOut, tempbuff, sizeof(analogOut));
			buff = analogOut;
			m_pb.unlock();
		}


	}
	//return 0;
	//pthread_exit(NULL);
}

void sendBackpacket(){
	Packets::EBUPacketAnalogOut sendBackPacket = Packets::EBUPacketAnalogOut();
	sendBackPacket.setChannelValue(5, AO_9);
	sendBackPacket.setChannelValue(5, AO_10);
	while (not timeToQuit){
		Packets::CawlPacket ut = Packets::CawlPacket(0, 0);
		memcpy(&ut.data, &sendBackPacket ,sizeof(sendBackPacket));
		if(m_cs.try_lock()){
			try{
				gatewaySocket.send(ut);
			}catch(int e){
				printf("ERROR number %i\n", e);
				perror("Description");
				printf("Packet not sent\n");
				m_cs.unlock();
				exit(1);
			}
			m_cs.unlock();
		}else{
			continue;
		}
	}
}
void sendPacket(){
	while(not timeToQuit){
		usleep(200000); 		//The usleep prevents the EBU from crashing...
		if(m_pb.try_lock()){
			ebuMan.sendAnalogCommand(buff, buff.getDestination());
			m_pb.unlock();
		}
	}
}


/*
 *TODO add support for receiving digital and relay packet info as well
 */
int main(void)
{
	signal(SIGINT, INT_handler);
	atexit(stopTheMachine);
	resetRelays();
	//set up for the relays that needs to be used for current functionality
	rPack.setRelayValue(R_A9,1);  		//9 and 10 are used for
	rPack.setRelayValue(R_A10,1);	// boom
	rPack.setRelayValue(R_A11,1);	//11 and 12 are used for bucket
	rPack.setRelayValue(R_A12,1);
	ebuMan.sendRelayCommand(rPack, 1);
	Packets::CawlPacket temp = Packets::CawlPacket(1,1);
	gatewaySocket.rec(temp);
	//gatewaySocket.setmetrics(true);
	//std::thread tOne (sendPacket);
	std::thread tTwo(recPacket);
	std::thread tThree (sendBackpacket);
	//tOne.join();
	tTwo.join();
	tThree.join();
	printf("Threads finished");
	ebuMan.sendAnalogCommand(stopPacket, 1);
	resetRelays();
	return 0;
}


