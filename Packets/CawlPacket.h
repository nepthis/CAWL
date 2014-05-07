/*
 * CawlPacket.h
 *
 *  Created on: Apr 17, 2014
 *      Author: cawl-mobile
 */

#include <stdint.h>
#include <cstring>

#ifndef CAWLPACKET_H
#define CAWLPACKET_H

namespace Packets {

class CawlPacket {
	public:
		CawlPacket();

		uint8_t getPrio();
		void setPrio(uint8_t);

		uint8_t getStreamId();
		void setStreamId(uint8_t);

		char getData();
		void setData(char);

		virtual ~CawlPacket();

	private:
		uint8_t prio, streamId;
		char data[256];

};

} /* namespace std */

#endif /* CAWLPACKET_H_ */
