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
	class EBUone one;
	class EBUtwo two;
	class Answer ans;
public:
	EBUMapper();
	virtual ~EBUMapper();
};


class EBUone{

public:
	EBUone();
	virtual ~EBUone();
};

class EBUtwo{

public:
	EBUtwo();
	virtual ~EBUtwo();

};


class Answer{
public:
	Answer();
	virtual ~Answer();
};

#endif /* EBUMAPPER_H_ */
