/*
 * CawlPacket.h
 *
 *  Created on: Apr 17, 2014
 *      Author: Håkan Therén
 *
 *      Description: Class to be used for storing data when sending in CawlSocket.cpp
 */

#ifndef CAWLPACKET_H
#define CAWLPACKET_H

#include <stdint.h>
#include <ctime>
#include <sys/time.h>
#include <string>
#include <cstring>


namespace Packets {

typedef struct CawlPacket{
	uint8_t prio, streamId, data_size;
	char data[256];
	time_t time_snd, time_arr,time_sndu,time_arru;

	CawlPacket(uint8_t prio = 0,uint8_t stream_id = 0, char* data =(char *)"");

	void SetData(char* data);
	char* GetData();

	void SetPrio(int Prio){prio=Prio;}
	uint8_t GetPrio(){return prio;}

	void SetId(int StreamId){streamId = StreamId;}
	int GetId(){return streamId;}

	void SetSnd();
	void SetRcv();
	time_t GetSnd(){return time_snd;}
	time_t GetRcv(){return time_arr;}
	time_t GetDelay();
	virtual ~CawlPacket();
}CawlPacket;

} /* namespace std */

#endif /* CAWLPACKET_H_ */
