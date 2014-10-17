/*
 * EBUTranslator.cpp
 *
 *  Created on: Sep 24, 2014
 *      Author: cawl-server
 */

#include "EBUTranslator.h"

namespace EBU {
//using namespace Packets;
EBUTranslator::EBUTranslator() {
	parkingBrake = true;
}
// Set the values in the data-struct destined for the first EBU (V-ECU)
void EBUTranslator::setEbuOne(SimPack* sp, AnalogOut* epaoOne, DigitalOut* epdoOne) {
	epaoOne->setDestination(1);
	epdoOne->setDestination(1);
	if (sp->getAnalog(BRAKEPEDAL) > 0.2){
		setBrakeLight(1, epdoOne);
	}else{
		setBrakeLight(0, epdoOne);
	}
}

// Set the values in the data-struct destined for the second EBU (V2-ECU)
void EBUTranslator::setEbuTwo(SimPack* sp, AnalogOut* epaoTwo, DigitalOut* epdoTwo) {
	epaoTwo->setDestination(2);
	epdoTwo->setDestination(2);
	//-----------------------------------Boom----------------------------------------------------
	setBoom(sp->getAnalog(LIFTSTICK), epaoTwo);
	//-----------------------------------Bucket--------------------------------------------------
	setBucket(sp->getAnalog(TILTSTICK), epaoTwo);
	//-----------------------Brake & Parking brake------------------------------------
	if(parkingBrake){
		setBrake(1.0, epaoTwo);
	}else{
		setBrake(sp->getAnalog(BRAKEPEDAL), epaoTwo);
	}
	//-------------------------------------Gas---------------------------------------------------
	setGas(sp->getAnalog(GASPEDAL), epaoTwo);
	//---------------------------------Steering--------------------------------------------------
	setSteer(sp->getAnalog(JOYSTICK),epaoTwo);
	//--------------------------------Oth. func.-------------------------------------------------
	setThirdFunc(sp->getAnalog(THIRDFUNCTION), epaoTwo);
	setFourthFunc(sp->getAnalog(FOURTHFUNCTION), epaoTwo);
	//-----------------------------------gears---------------------------------------------------
	setGear(sp->getDigital(GEARCLCFORWARD),sp->getDigital(GEARCLCREVERSE),epdoTwo);
	//-----------------------------------CDC-Activation------------------------------------------
	setCDC(sp->getDigital(ACTIVATIONCDC), epdoTwo);
	//---------------------------------"Parkingbrake"--------------------------------------------
	//This is actually not for the REAL parkingbrake, it just has the kind of same effect for now.
	//Should be changed into the real one later on the wheel loader. Lot's of work...
	setPBrake(sp->getDigital(PARKINGBRAKE));
}
void EBUTranslator::setBoom(float value, AnalogOut* pkt) {
	//4.5V is max, 0.5 min. signal is sent over two pins, inverted. 2.5V is "0" on both pins
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(5.0-temp, AO_9);
	pkt->setChannelValue(temp, AO_10);
}
void EBUTranslator::setBucket(float value, AnalogOut* pkt) {
	//4.5V is max, 0.5 min. signal is sent over two pins, inverted. 2.5V is "0" on both pins
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(5-temp, AO_11);
	pkt->setChannelValue(temp, AO_12);
}
void EBUTranslator::setGas(float value, AnalogOut* pkt) {
	float temp = value*4.0+0.5;	//4.5V is max, 0.5 min. signal is sent over two pins, inverted
	pkt->setChannelValue(5.0-temp, AO_19);
	pkt->setChannelValue((temp), AO_20);
}
void EBUTranslator::setSteer(float value, AnalogOut* pkt) {
	//4.5V is max, 0.5 min. signal is sent over two pins, inverted. 2.5V is "0" on both pins
	float temp = value  * 2.0 + 2.5;
	pkt->setChannelValue(temp, AO_17);
	pkt->setChannelValue(5.0-temp, AO_18);
}
void EBUTranslator::setBrake(float value, AnalogOut* pkt) {
	float temp = value*3.0; //pretty close to max Amp for the solenoid used
	pkt->setChannelValue(temp, AO_7);
}
void EBUTranslator::setBrakeLight(int onOff, DigitalOut *pkt){
	pkt->setDigitalOut(SO7_HB56, onOff);
}
void EBUTranslator::setHorn(int onOff, DigitalOut *pkt){
	pkt->setDigitalOut(SO5_HB54, onOff);
}
void EBUTranslator::setCDC(int onOff, DigitalOut *pkt){
	pkt->setDigitalOut(DO12_EA34, onOff);
}
void EBUTranslator::setGear(int  forward, int reverse, DigitalOut* pkt){
	pkt->setDigitalOut(DO22_EA61, reverse);
	pkt->setDigitalOut(DO31_EA52, forward);
}
void EBUTranslator::setThirdFunc(float value, AnalogOut* pkt) {
	//4.5V is max, 0.5 min. signal is sent over two pins, inverted. 2.5V is "0" on both pins
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(5.0-temp, AO_13);
	pkt->setChannelValue(temp, AO_14);
}
void EBUTranslator::setFourthFunc(float value, AnalogOut* pkt) {
	//4.5V is max, 0.5 min. signal is sent over two pins, inverted. 2.5V is "0" on both pins
	float temp = value * 2.0 + 2.5;
	pkt->setChannelValue(temp, AO_15);
	pkt->setChannelValue(5.0-temp, AO_16);
}
//This is actually not for the REAL parkingbrake, it just has the kind of same effect for now.
void EBUTranslator::setPBrake(int onOff) {
	if(onOff == 1){parkingBrake = true;}
	else if (onOff == 0){parkingBrake = false;}
}

EBUTranslator::~EBUTranslator() {
}

} /* namespace Packets */
