/*
 * GatherMetrics.cpp
 *
 *  Created on: Jun 16, 2014
 *      Author: Robin Bond
 */

#include "GatherMetrics.h"

namespace Netapi {

GatherMetrics::GatherMetrics() {
	//Start database workers (5 threads)
	if(database->start(5)){
		db_init = true;
	}else{
		db_init = false;
	}
}

/*
 * Inserts measurement, adds value to worker queue for insert to database
 */
int GatherMetrics::setMeasure(int measurement, float value) {

	if(db_init == false)return -1;

	if (measurement == STATUS){
		measurementData data;
		std::time_t tim = std::time(nullptr);
		std::string timt = asctime(std::localtime(&tim));
		data.name = "SRTT";
		data.type = "SRTT - " + (std::string)timt;
		data.timeStamp= timt;
		data.id = "0";
		data.value = std::to_string(value);
		database->insert(data);

		return 1;
	}else{
		return -1;
	}
}

GatherMetrics::~GatherMetrics() {
	// TODO Auto-generated destructor stub
}

} /* namespace Netapi */
