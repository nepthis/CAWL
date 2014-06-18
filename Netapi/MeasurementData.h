/*
 * MeasurementData.h
 * Description: container class
 *  Created on: Jun 18, 2014
 *      Author: Robin Bond
 */

#ifndef MEASUREMENTDATA_H_
#define MEASUREMENTDATA_H_
#include <string>

typedef struct measurementData{
	std::string id;
	std::string name;
	std::string timeStamp;
	std::string type;
	std::string value;
}measurementData;


#endif /* MEASUREMENTDATA_H_ */
