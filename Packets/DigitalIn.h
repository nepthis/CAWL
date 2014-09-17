/*
 * DigitalIn.h
 *
 *  Created on: Sep 17, 2014
 *      Author: cawl-server
 */

#ifndef DIGITALIN_H_
#define DIGITALIN_H_

namespace Packets {

class DigitalIn {
	int source;
public:
	DigitalIn();
	void setSource(int ebuNum);
	int getSource();
	virtual ~DigitalIn();
};

} /* namespace Packets */

#endif /* DIGITALIN_H_ */
