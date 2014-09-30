/*
 * EBUTranslator.h
 *
 *  Created on: Sep 24, 2014
 *      Author: cawl-server
 */

#ifndef EBUTRANSLATOR_H_
#define EBUTRANSLATOR_H_

#include "../Packets/AnalogOut.h"
#include "../Packets/DigitalOut.h"
#include "../Packets/SimPack.h"

namespace EBU {
using namespace Packets;
class EBUTranslator {
private:
	void setBoom(float value, Packets::AnalogOut* pkt);
	void setBucket(float value, Packets::AnalogOut* pkt);
	void setGas(float value, Packets::AnalogOut* pkt);
	void setBrake(float value, Packets::AnalogOut* pkt);
	void setSteer(float value, Packets::AnalogOut* pkt);
	void setGear(int  forward, int reverse, Packets::DigitalOut* pkt);
	void setBrakeLight(int onOff, Packets::DigitalOut *pkt);
	void setHorn(int onOff, Packets::DigitalOut *pkt);
	void setThirdFunc(float value, Packets::AnalogOut *pkt);
	void setFourthFunc(float value, Packets::AnalogOut *pkt);
public:
	EBUTranslator();
	void setEbuOne(SimPack* sp, AnalogOut* epaoOne,  DigitalOut* epdoOne);
	void setEbuTwo(SimPack* sp, AnalogOut* epaoTwo, DigitalOut* epdoTwo);
	virtual ~EBUTranslator();
};

} /* namespace Packets */

#endif /* EBUTRANSLATOR_H_ */