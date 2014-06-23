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
#include <mutex>
#include <thread>

#include <stdint.h> //Who doesn'y like ints?
#include <errno.h>	//For a huge list of errors

#include <netinet/in.h> //For UDP
#include <netdb.h> // in_addr_t

#include "../Netapi/CawlSocket.h" 							//For communication between "gateways"
#include"../EBU/EBUManager.h"								//For communication to EBU
#include "../Packets/CawlPacket.h"							//Struct/class for packets between "gateways"
#include "../Packets/EBUPacketAnalogOut.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/EBUPacketAnalogOut.h"	//In order to set which relays easier, it contains all defines.


namespace Major_Tom {

class Mobile {
public:
	Mobile();												//Constructor
	void startUp(bool condition);	//For starting the connection to the other "gateway"
	void socketReceive(); 					//Receiving data from socket
	void socketSend(); 						//Sending data back through socket
	void ebuSend(); 								//Send data to the EBU
	virtual ~Mobile();							//Destructor

private:
	Packets::EBUPacketAnalogOut stopPacket;
	Packets::EBURelayPacket rPack;
	Netapi::Host h;
	Netapi::CawlSocket gatewaySocket;
	std::queue<Packets::CawlPacket> q_cawlBuffer;
	std::mutex m_Queue;		//Conflicts will occuf if it's possible to insert and remove elements at the same time
	std::mutex m_Cawl;		//For the Socket in order to be able to send and receive
	std::condition_variable cond;
	bool ready;
	bool pleased;
	EBU::EBUManager em;
};

} /* namespace Major_Tom */

#endif /* MOBILE_H_ */
