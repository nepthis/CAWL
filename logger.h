/*
 * logger.h
 *
 *  Created on: Nov 10, 2014
 *      Author: cawl-server
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <fstream>
#include <mutex>
#include <stdio.h>
#include <ctime>
#include <string>
#include <iostream>
#define LOGFILE "./cawl_logfile.txt"

extern std::string logLevel;
void  logError( const std::string &text );
void logVerbose(const std::string &text);
void logWarning(const std::string &text);


#endif /* LOGGER_H_ */
