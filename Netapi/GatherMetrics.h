/*
 * GatherMetrics.h
 *	 Description:
 *	 This object measures data and inserts it into the database
 *  Created on: Jun 16, 2014
 *      Author: Robin Bond
 */

#ifndef GATHERMETRICS_H_
#define GATHERMETRICS_H_
#include <vector> 	//for storing metrics data
#include <time.h> 	//to create a referene point for measurements
#include <utility> 	//used for pairs
#include <string> 	//used for ID and tags
#include <fstream>	//used for opening/reading error files from sctp
#include <stdexcept> //used for exceptions
#include "../Packets/CawlPacket.h" //for accessing the delay
#include "MeasurementData.h"

namespace Netapi {
typedef struct measureOpt{
	bool DELAY;
	bool CHKSUMERR;
};
class GatherMetrics {
	measureOpt options;
	//database object
public:
	GatherMetrics();
	std::vector  measuredata(Packets::CawlPacket, int testID, std::string name);
	void setOption(std::string measurement, bool value);
	virtual ~GatherMetrics();
};

} /* namespace Netapi */

#endif /* GATHERMETRICS_H_ */
