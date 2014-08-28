/*
 * Mobile.h
 *
 *  Created on: Jun 23, 2014
 *      Author: Robin Bond
 */

#ifndef MOBILE_H_
#define MOBILE_H_

#define REC_PORT 56565
#define REC_ADDR "0.0.0.0"
//For threads and mutex
#include <chrono>
#include <mutex>	//std::mutex
#include <stdint.h> //Who doesn't like ints?
#include <string>	//Standard string
#include <errno.h>	//For a huge list of errors
#include <queue> //Used as a buffer
#include <unistd.h>
#include <netinet/in.h> //For UDP
#include <netdb.h> // in_addr_t

#include"../EBU/EBUManager.h"				//For communication to EBU
#include "../Simulator/Sim.h"
#include "../Packets/EBUPacketAnalogOut.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/EBUPacketDigitalOut.h"	//In order to set which relays easier, it contains all defines.


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
	Mobile();	//Constructor
	void socketReceive(); 				//Receiving data from an UDP socket, port 65656
	void socketSend(); 					//Sending data back through socket
	void ebuSend(); 							//Send data to the EBU
	void setBoom(float value, Packets::EBUPacketAnalogOut* pkt);
	void setBucket(float value, Packets::EBUPacketAnalogOut* pkt);
	void setGas(float value, Packets::EBUPacketAnalogOut* pkt);
	void setBrake(float value, Packets::EBUPacketAnalogOut* pkt);
	void setSteer(float value, Packets::EBUPacketAnalogOut* pkt);
	void setGear(int p1,int  p2, int p3, Packets::EBUPacketDigitalOut* pkt);
	void setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao, Packets::EBUPacketDigitalOut* epdo);
	void setEbuTwo(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao, Packets::EBUPacketDigitalOut* epdo);
	virtual ~Mobile();							//Destructor

private:
	socklen_t slen;
	int mobSocket;	//Socket for mobile client, will listen for packages on port 65656
	struct sockaddr_in mobAddr;
	Packets::EBUPacketAnalogOut stopPacket;
	Packets::EBURelayPacket rPackOne;
	Packets::EBURelayPacket rPackTwo;
	Packets::SimPack state;
	std::queue<Packets::SimPack> q_cawlBuffer;
	std::mutex m_Queue;		//to prevent conflicts of two threads accessning the same object.
	EBU::EBUManager em;
};

} /* namespace Major_Tom */

#endif /* MOBILE_H_ */
