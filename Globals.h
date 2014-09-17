/*
 * Globals.h
 *
 *  Created on: Sep 17, 2014
 *      Author: cawl-server
 */

#ifndef GLOBALS_H_
#define GLOBALS_H_
#include <mutex>	//std mutex and unique locks
using namespace std;
mutex m_Queue;
mutex m_imuState;
mutex m_state;




#endif /* GLOBALS_H_ */
