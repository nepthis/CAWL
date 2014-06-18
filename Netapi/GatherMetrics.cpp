/*
 * GatherMetrics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: Robin Bond
 */

#include "GatherMetrics.h"

namespace Netapi {

GatherMetrics::GatherMetrics() {
	options = measureOpt{false,false};

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

std::vector GatherMetrics::measuredata(Packets::CawlPacket cawlPacket,
		int testID, std::string testName) {
	//std::vector<std::vector<std::pair<std::string,std::string>(5)> multiVector;
	time_t rawtime;
	struct tm * now;
	time(&rawtime);
	now = localtime(&rawtime); //Tue Jun 17 12:50:43 2014
	if (not (options.CHKSUMERR or options.DELAY)){
		throw std::logic_error("No options set");
	}else{
		if(options.DELAY){
			//id (type of test), name (for this test), timestamp, type (RTT, BER, etc), data (value for the type)
			measurementData delayData =  {std::to_string(testID), testName, asctime(now),"DELAY", std::to_string(cawlPacket.GetDelay())};
			//database object insert RTT vector.

		}
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
			measurementData chksumerrData =  {std::to_string(testID), testName, asctime(now),"CHKSUMERR", line};
			//database object insert RTT vector.
		}
	}
	throw std::logic_error("Unable to measure data");

}

GatherMetrics::~GatherMetrics() {
	// TODO Auto-generated destructor stub
}

} /* namespace Netapi */
