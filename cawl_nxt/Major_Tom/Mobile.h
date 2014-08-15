/*
 * Mobile.h
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */

#ifndef MOBILE_H_
#define MOBILE_H_
//For threads and mutex
#include <chrono>
#include <mutex>	//std::mutex
#include <stdint.h> //Who doesn'y like ints?
#include <string>	//Standard string
#include <errno.h>	//For a huge list of errors

#include <netinet/in.h> //For UDP
#include <netdb.h> // in_addr_t

#include "../Netapi/CawlSocket.h" 			//For communication between "gateways"
#include"../EBU/EBUManager.h"				//For communication to EBU
#include "../Packets/CawlPacket.h"			//Struct/class for packets between "gateways"
#include "../Packets/EBUPacketAnalogOut.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/EBUPacketAnalogOut.h"	//In order to set which relays easier, it contains all defines.


namespace Major_Tom {
/*	This class is responsible for the communication between the wheel loader's EBUs and the to the Ground running
 * 	with the simulator. This class will receive data over the cawlsocket and then use an EBUManager to send the
 * 	EBUPacketAnalogOut that lies within the data field in the CawlPacket. The analogPacket is put inside a buffer
 * 	by the socketReceive method and emptied and sent to the EBU with the ebuSend method.
 *
 */
class Mobile {
public:
	bool pleased;
	Mobile(char* addressOne, char* addressTwo);	//Constructor
	void startUp();								//For starting the connection to the other "gateway"
	void socketReceive(); 						//Receiving data from socket
	void socketSend(); 							//Sending data back through socket
	void ebuSend(); 							//Send data to the EBU
	virtual ~Mobile();							//Destructor

private:
	Packets::EBUPacketAnalogOut stopPacket;
	Packets::EBURelayPacket rPack;
	Netapi::Host h1;
	Netapi::Host h2;
	Netapi::CawlSocket *gatewaySocketSend;
	Netapi::CawlSocket *gatewaySocketReceive;
	std::queue<Packets::EBUPacketAnalogOut> q_cawlBuffer;
	std::mutex m_Queue;		//Conflicts will occuf if it's possible to insert and remove elements at the same time
	EBU::EBUManager em;
};

} /* namespace Major_Tom */

#endif /* MOBILE_H_ */
