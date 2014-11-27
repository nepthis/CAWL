/*
 * SimPack.h
 *
 *  Created on: May 20, 2014
 *      Author: cawl-server
 */

#ifndef SIMPACK_H_
#define SIMPACK_H_
#include <stdint.h>
#include <chrono>


//Analog signals floats
#define GASPEDAL 0			//0.0 - 1.0
#define BRAKEPEDAL 1		//0.0 - 1.0
#define LIFTSTICK 2			//(-1.0) <-(-0.0)-> (1.0)
#define TILTSTICK 3			//(-1.0) <-(-0.0)-> (1.0)
#define THIRDFUNCTION 4		//(-1.0) <-(-0.0)-> (1.0)
#define FOURTHFUNCTION 5	//(-1.0) <-(-0.0)-> (1.0)
#define STEERINGWHEEL 6		//(-1.0) <-(-0.0)-> (1.0)
#define JOYSTICK 7			//(-1.0) <-(-0.0)-> (1.0)

//Digital signals 0/1
#define GEARSELECT1 0
#define GEARSELECT2 1
#define GEARSELECT3 2
#define GEARSTICKFOR 3
#define GEARSTICKREV 4
#define ATTACHEMENTL 5
#define PARKINGBRAKE 6
#define HORN 7
#define DIRINDILEFT 8
#define DIRINDIRIGHT 9
#define HIGHBEAMS 10
#define BUCKETPOSITIONERACTV 11
#define BUCKETPOSITIONERPROG 12
#define BOOMKICKOUTACTV 13
#define BOOMKICKOUTPROG 14
#define RETURNTODIGACTV 15
#define RETURNTODIGPROG 16
#define FLOATINGPOSITION 17
#define SINGLEACTINGLIFTINGFUNC 18
#define BOOMSUSPENSIONGEAR 19
#define BOOMSUSPENSIONSPEED 20
#define WARNINGBEACON 21
#define IGNITIONPOS1 22
#define IGNITIONPOS2 23
#define SAFETYLOCKCDC 24
#define ACTIVATIONCDC 25
#define KICKDOWNCDC 26
#define CLCLOCKOUT 27
#define ACTIVATIONCLC 28
#define GEARCLCREVERSE 29
#define GEARCLCFORWARD 30
#define KICKDOWNCLC 31


namespace Packets {
/*	Container class for data received from the simulator
 * 	Setters and getters for the data exist to abastract away
 * 	the array and variables.
 */
typedef struct fromSim {
	uint32_t packetId;
	uint32_t packetSize;
	float analog[8];
	uint32_t digital;
}fromSim;

class SimPack {
public:
	fromSim fs;
	SimPack();
	void setID(uint32_t id);
	uint32_t getPacketSize(void);
	int getDigital(int);
	int setDigital(int,int);
	float getAnalog(int);
	int setAnalog(int, float);
	bool operator==(const SimPack &s);
	virtual ~SimPack();
	void stampTime();
	fromSim getData(void);
};

} /* namespace Packets */

#endif /* SIMPACK_H_ */
