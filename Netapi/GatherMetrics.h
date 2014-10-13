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
#include <ctime>
#include <utility> 	//used for pairs
#include <string> 	//used for ID and tags
#include <fstream>	//used for opening/reading error files from sctp
#include <stdexcept> //used for exceptions
#include <iostream> //for printing
#include "MeasurementData.h"
#include "../Db/mysqlconnector.h"

#define SRTT 0



namespace Netapi {

class GatherMetrics {
	Db::mysqlconnector *database = new Db::mysqlconnector();
	bool db_init;
public:
	GatherMetrics();
	int setMeasure(int measurement, float value);
	virtual ~GatherMetrics();
};

} /* namespace Netapi */

#endif /* GATHERMETRICS_H_ */
