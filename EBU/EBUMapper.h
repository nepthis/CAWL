/*
 * EBUMapper.h
 *
 *  Created on: Apr 16, 2014
 *      Author: Robin Bond
 *  Description:
 *  	The mapper class will be able to map pins and values to the correct
 *  	EBU and format.
 */
#include <stdlib.h>

#ifndef EBUMAPPER_H_
#define EBUMAPPER_H_

class EBUMapper {
	struct mapper ebuOne, ebuTwo;
	double sentPackages, recPackages;
public:
	EBUMapper();
	virtual ~EBUMapper();
	struct answer mapData(uint16_t pin);
};

typedef struct mapper{

};

typedef struct answer{

};

#endif /* EBUMAPPER_H_ */
