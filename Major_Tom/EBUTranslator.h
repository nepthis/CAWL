/*
 * EBUTranslator.h
 *
 *  Created on: Sep 24, 2014
 *      Author: Robin Bond
 *      Description:
 *     Contains functions for translating data from the simpack back into a voltage
 *     the voltage is different from function to function and is represented by a 16-bit
 *     integer for the analog values while the digital signals just take 1 or 0.
 */

#ifndef EBUTRANSLATOR_H_
#define EBUTRANSLATOR_H_

//#include "FromEBU.h"
#include "../Packets/AllPackets.h"
#include <stdio.h>
#include <cmath>


namespace EBU {
using namespace Packets;
class EBUTranslator {
private:
	bool parkingBrake;
	void setPBrake(int onOff);
	void setBoom(float value, Packets::AnalogOut* pkt);
	void setBucket(float value, Packets::AnalogOut* pkt);
	void setGas(float value, Packets::AnalogOut* pkt);
	void setBrake(float value, Packets::AnalogOut* pkt);
	void setSteer(float value, Packets::AnalogOut* pkt);
	void setGear(int  forward, int reverse, Packets::DigitalOut* pkt);
	void setBrakeLight(int onOff, Packets::DigitalOut *pkt);
	void setHorn(int onOff, Packets::DigitalOut *pkt);
	void setCDC(int onOff, Packets::DigitalOut *pkt);
	void setThirdFunc(float value, Packets::AnalogOut *pkt);
	void setFourthFunc(float value, Packets::AnalogOut *pkt);
	void getAngle( );
public:
	EBUTranslator();
	void setEbuOne(SimPack* sp, AnalogOut* epaoOne,  DigitalOut* epdoOne);
	void setEbuTwo(SimPack* sp, AnalogOut* epaoTwo, DigitalOut* epdoTwo);
//	void readEbuOne(fromEBU *data, AnalogIn* epaiOne, DigitalIn* epdiOne);
//	void readEbuTwo(fromEBU *data, AnalogIn* epaiTwo, DigitalIn* epdiTwo);
	virtual ~EBUTranslator();
};

} /* namespace Packets */

#endif /* EBUTRANSLATOR_H_ */
