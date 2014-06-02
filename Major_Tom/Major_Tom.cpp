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
	exit(EXIT_SUCCESS);
}
/*
 * Resets all relays to 0 meaning that everything should stop
 */
void resetRelays(void){
	rPack = Packets::EBURelayPacket();
	ebuMan.sendRelayCommand(rPack, 1);
	//ebuMan.sendRelayCommand(rPack, 2);
}


/*
 * Takes the front element out from the buffer and sends it to the right
 * EBU with a EBUPacketAnalogOut based on data found in the CawlPacket.
 * WARNING: NOT IN USE
 */
//void *sendToEBU(void *parg){
//	//	uint8_t type;
//	//	int pin;
//	//	int value;
//	//	int ebu;
//	while(!timeToQuit){
//		//The sleep is to make sure that we don't flood the EBU, and since the EBU responds with 10Hz then we will do the same.
//		usleep(100000);
//		pthread_mutex_lock(&m_PacketAnalogOut);
//		pthread_mutex_lock(&m_ebuMan);
//		if (packetBuffer.empty()){
//			//Send stop to all EBUs and all types
//			packetAnalogOut = stopPacket;
//			ebuMan.sendAnalogCommand(packetAnalogOut, 1);
//		}else{
//			cPack = packetBuffer.front();
//			printf("\nContents of datapack: %s\n", cPack.data);
//			packetBuffer.pop();
//			//			memcpy(&type, cPack.data,8);
//			//			memcpy(&pin, cPack.data+8, 16);
//			//			memcpy(&value, cPack.data+24, 16);
//			//			memcpy(&ebu, cPack.data+40,16);
//			printf("\nPacket data contains: %i, %i, %i, %i\n", dc.type, dc.pin, dc.value,dc.ebuNum);
//			printf("\nContents of datapack: %s\n", cPack.data);
//			memcpy(&dc, cPack.data, sizeof(dc));
//			printf("\nPacket data contains: %i, %i, %i, %i\n", dc.type, dc.pin, dc.value,dc.ebuNum);
//			switch (dc.type){
//			case 1:
//				packetAnalogOut.setChannelValue(dc.value, dc.pin);
//				ebuMan.sendAnalogCommand(packetAnalogOut, dc.ebuNum);
//			}
//		}
//		pthread_mutex_unlock(&m_ebuMan);
//		pthread_mutex_unlock(&m_PacketAnalogOut);
//	}
//	pthread_exit(NULL);
//}
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
void *recPacket(void *parg){
	Packets::CawlPacket recPack =  Packets::CawlPacket(0);
	Packets::EBUPacketAnalogOut analogOut =  Packets::EBUPacketAnalogOut();
	while(not timeToQuit){
		try{
			gatewaySocket.rec(recPack);
		}catch(int e){
			printf("ERROR %i\n", e);
			resetRelays();
			perror("Nope");
		}
		pthread_mutex_lock(&m_packetBuffer);

		char *tempbuff;
		tempbuff = (char*) malloc(sizeof(analogOut));
		memcpy(tempbuff, recPack.data, sizeof(analogOut));
		memcpy(&analogOut, tempbuff, sizeof(analogOut));
		buff = analogOut;
		//		if(compPack(analogOut)){
		//			buff = analogOut;
		//		}
		pthread_mutex_unlock(&m_packetBuffer);

	}
	pthread_exit(NULL);
}


void *sendPacket(void *parg){

	while(not timeToQuit){
		usleep(200000);
		pthread_mutex_lock(&m_packetBuffer);
		//		if(packetBuffer.empty()){
		//			continue;
		//			//ebuMan.sendAnalogCommand(stopPacket, 1);
		//			//ebuMan.sendAnalogCommand(stopPacket, 2);
		//		}else{
		//sendPack = packetBuffer.front();
		//packetBuffer.pop();
		//printf("Values in the analog packet to be sent;\n%i, %i, %i, %i\n", buff.getChannelValue(AO_9),buff.getChannelValue(AO_10),buff.getChannelValue(AO_11),buff.getChannelValue(AO_12));
		ebuMan.sendAnalogCommand(buff, buff.getDestination());
		//printf("Packet sent to ebu1\n");
		//}
		pthread_mutex_unlock(&m_packetBuffer);
	}
	pthread_exit(NULL);
}


/*
 * One thread for receiving through the CawlSocket and pushing packets into a
 * buffer.
 * Another thread will empty the buffer and send the packet to the right EBU
 * with the right values.
 *TODO add support for receiving digital and relay packet info as well
 */
int main(void)
{
	signal(SIGINT, INT_handler);
	atexit(resetRelays);
	resetRelays();
	rPack.setRelayValue(R_A9,1);
	rPack.setRelayValue(R_A10,1);
	rPack.setRelayValue(R_A11,1);
	rPack.setRelayValue(R_A12,1);
	ebuMan.sendRelayCommand(rPack, 1);
	Packets::CawlPacket temp = Packets::CawlPacket(1,1);
	gatewaySocket.rec(temp);
	pthread_t t1;
	pthread_t t2;
	pthread_create(&t1, NULL, sendPacket, NULL);
	pthread_create(&t2, NULL, recPacket, NULL);
	pthread_join(t1,NULL);
	pthread_join(t2,NULL);
	printf("Threads finished");
	resetRelays();
	return 0;
}


