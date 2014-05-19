#include "Major_Tom.h"

//Används för ctrl+c, stänger av allt på ett bättre sätt.
void INT_handler(int dummy){
	timeToQuit = 1;
	exit(EXIT_SUCCESS);
}


/*
 * Takes the front element out from the buffer and sends it to the right
 * EBU with a EBUPacketAnalogOut based on data found in the CawlPacket.
 */
void *sendToEBU(void *parg){
//	uint8_t type;
//	int pin;
//	int value;
//	int ebu;
	while(!timeToQuit){
		//The sleep is to make sure that we don't flood the EBU, and since the EBU responds with 10Hz then we will do the same.
		usleep(100000);
		pthread_mutex_lock(&m_PacketAnalogOut);
		pthread_mutex_lock(&m_ebuMan);
		if (packetBuffer.empty()){
			//Send stop to all EBUs and all types
			packetAnalogOut = stopPacket;
			ebuMan.sendAnalogCommand(packetAnalogOut, 1);
		}else{
			cPack = packetBuffer.front();
			printf("\nContents of datapack: %s\n", cPack.data);
			packetBuffer.pop();
//			memcpy(&type, cPack.data,8);
//			memcpy(&pin, cPack.data+8, 16);
//			memcpy(&value, cPack.data+24, 16);
//			memcpy(&ebu, cPack.data+40,16);
			printf("\nPacket data contains: %i, %i, %i, %i\n", dc.type, dc.pin, dc.value,dc.ebuNum);
			printf("\nContents of datapack: %s\n", cPack.data);
			memcpy(&dc, cPack.data, sizeof(dc));
			printf("\nPacket data contains: %i, %i, %i, %i\n", dc.type, dc.pin, dc.value,dc.ebuNum);
			switch (dc.type){
			case 1:
				packetAnalogOut.setChannelValue(dc.value, dc.pin);
				ebuMan.sendAnalogCommand(packetAnalogOut, dc.ebuNum);
			}
		}
		pthread_mutex_unlock(&m_ebuMan);
		pthread_mutex_unlock(&m_PacketAnalogOut);
	}
	pthread_exit(NULL);
}
void *receiveFromGateway(void *parg){
	while(not timeToQuit){
		//pthread_mutex_lock(&m_gwSocket);
		pthread_mutex_lock(&m_packetBuffer);
		pthread_mutex_lock(&m_cawlPacket);
		gatewaySocket.rec(cPack);
		//Packets::CawlPacket temp = cPack;
		//packetBuffer.push(temp);
		memcpy(&dc, cPack.data, sizeof(dc));
		packetAnalogOut.setChannelValue(dc.value, dc.pin);
		ebuMan.sendAnalogCommand(packetAnalogOut, dc.ebuNum);
		pthread_mutex_unlock(&m_cawlPacket);
		pthread_mutex_unlock(&m_packetBuffer);
		//pthread_mutex_unlock(&m_gwSocket);
	}
	pthread_exit(NULL);
}

/*
 * Resets all relays to 0, that is everything turns off.
 */
void resetRelays(void){
	rPack = Packets::EBURelayPacket();
	ebuMan.sendRelayCommand(rPack, 1);
	//ebuMan.sendRelayCommand(rPack, 2);
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
	sleep(1);
	//pthread_t t1;
	pthread_t t2;
	//pthread_create(&t1, NULL, sendToEBU, NULL);
	pthread_create(&t2, NULL, receiveFromGateway, NULL);
	pthread_join(t2,NULL);
	printf("Threads finished");
	return 0;
}


