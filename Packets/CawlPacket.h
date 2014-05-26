/*
 * CawlPacket.h
 *
 *  Created on: Apr 17, 2014
 *      Author: Håkan Therén
 *
 *      Description: Class to be used for storing data when sending in CawlSocket.cpp
 */

#include <stdint.h>
#include <ctime>


#ifndef CAWLPACKET_H
#define CAWLPACKET_H

namespace Packets {
/*
class CawlPacket {

public:
	uint8_t prio, streamId;
	char * data;
	CawlPacket(uint8_t prio = 0,uint8_t stream_id = 0, char* data =(char *)"");

	virtual ~CawlPacket();

};

*/

typedef struct CawlPacket{
	uint8_t prio, streamId, data_size;
	char data[256];
	time_t time_snd, time_arr;

	CawlPacket(uint8_t prio = 0,uint8_t stream_id = 0, char* data =(char *)"");
	virtual ~CawlPacket();
	void SetData(char* data);
	char* GetData();
}CawlPacket;

} /* namespace std */

#endif /* CAWLPACKET_H_ */
