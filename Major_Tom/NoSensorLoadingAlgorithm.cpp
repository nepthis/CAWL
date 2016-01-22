#include "NoSensorLoadingAlgorithm.h"




void NoSensorLoadingAlgorithm::start() {
	// Drive Forward
	EBUTranslator::setGear(1, 0 , Packets::DigitalOut* pkt); // Set gear to forward
	EBUTranslator::setPBrake(0); // Set brake off
	EBUTranslator::setGas(DEFAULT, AnalogOut* pkt); // Press gas pedal
	usleep(1000);
	EBUTranslator::setGas(0, AnalogOut* pkt); // Release gas pedal
	EBUTranslator::setBrake(DEFAULT, AnalogOut* pkt); // Press brake pedal

	// Perform lift
	EBUTranslator::setBucket(DEFAULT, AnalogOut* pkt); // Start tilting bucket
	usleep(1000);
	EBUTranslator::setBucket(0, AnalogOut* pkt); // Stop tilting bucket
	EBUTranslator::setBoom(DEFAULT, AnalogOut* pkt); // Start lifting boom
	usleep(2000);
	EBUTranslator::setBoom(0, AnalogOut* pkt); // Stop lifting boom

	// Drive backwards
	EBUTranslator::setGear(0, 1, Packets::DigitalOut* pkt); // Set gear to reverse
	EBUTranslator::setBrake(0, AnalogOut* pkt); // Release break
	EBUTranslator::setGas(DEFAULT, AnalogOut* pkt); // Press gas pedal
	usleep(1000);
	EBUTranslator::setGas(0, AnalogOut* pkt); // Release gas pedal
	EBUTranslator::setBrake(DEFAULT, AnalogOut* pkt); // Press brake pedal
	usleep(1000);
	EBUTranslator::setPBrake(1); // Set brake on
}


