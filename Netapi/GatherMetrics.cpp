/*
 * GatherMetrics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: Robin Bond
 */

#include "GatherMetrics.h"

namespace Netapi {

GatherMetrics::GatherMetrics() {
	database->start(5); //Number of worker threads
}


void GatherMetrics::setOption(std::string measurement, bool value) {
	if (measurement=="CHKSUMERR"){
		options.CHKSUMERR = value;
		return;
	}
	if (measurement == "DELAY"){
		options.DELAY = value;
		return;
	}
}

void GatherMetrics::measuredata(Packets::CawlPacket cawlPacket,
		int testID, std::string testName) {
	time_t rawtime;
	struct tm * now;
	time(&rawtime);
	now = localtime(&rawtime); //Tue Jun 17 12:50:43 2014
	if (not (options.CHKSUMERR or options.DELAY)){
		throw std::logic_error("No options set");
	}else{
		if (options.CHKSUMERR){
			std::string line;
			std:: ifstream chksum ("/proc/net/sctp/SctpChecksumErrors");
			if (chksum.is_open()) {
				while ( getline (chksum,line) )
				{
					std::cout << line << " amount of checksum errors\n";
				}
				chksum.close();
			}else{
				line = "0";
			}
			std::string chkID = std::to_string(testID);
			measurementData chksumerrData = measurementData{chkID, testName, asctime(now),"CHKSUMERR", line};

			database->insert(chksumerrData);
		}
	}
	//throw std::logic_error("Unable to measure data");

}

void GatherMetrics::measureDelay(Packets::CawlPacket packet, int testID,
		std::string name) {
	time_t rawtime;
	struct tm * now;
	time(&rawtime);
	now = localtime(&rawtime); //Tue Jun 17 12:50:43 2014
	if(options.DELAY){
		//id (type of test), name (for this test), timestamp, type (RTT, BER, etc), data (value for the type)
		std::string tempTime = std::to_string(packet.GetDelay());
		measurementData delayData = measurementData{std::to_string(testID), name, asctime(now),"DELAY", tempTime};
		database->insert(delayData);
		std::cout << std::to_string(testID) << name << asctime(now) << "DELAY" <<tempTime<< '\n';
	}else{
		throw std::logic_error("Options not set");
	}


}

GatherMetrics::~GatherMetrics() {
	// TODO Auto-generated destructor stub
}

} /* namespace Netapi */
