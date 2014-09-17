/*
 * Filter.h
 *
 *  Created on: Sep 10, 2014
 *      Author: cawl-server
 */

#ifndef FILTER_H_
#define FILTER_H_

namespace Packets {

class Filter {
private:
float preValues[6] = 0.0;
public:
	Filter();
	float filterLow(float newValue, float frequency, int type);
	float filterHigh(float newValue, float frequency, int type);
	virtual ~Filter();
};

} /* namespace Packets */

#endif /* FILTER_H_ */
