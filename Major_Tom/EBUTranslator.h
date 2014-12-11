/*
 * EBUTranslator.h
 *  Created on: Sep 24, 2014
 *  Author: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */
#ifndef EBUTRANSLATOR_H_
#define EBUTRANSLATOR_H_

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
