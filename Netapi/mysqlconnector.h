/*
 * mysqlconnector.h
 *
 *  Created on: Jun 17, 2014
 *      Author: cawl-mobile
 */

#ifndef MYSQLCONNECTOR_H_
#define MYSQLCONNECTOR_H_

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <stdio.h>

//#include "../Netapi/MeasurementData.h"


// Database credentials etc.
#define mysql_address   "127.0.0.1"
#define port            "3306"
#define user            "cawl"
#define password        "ultrasupergurksallad"
#define db              "cawl"
#define tbl             "tests"

typedef struct measurementData{
	std::string id;
	std::string name;
	std::string timeStamp;
	std::string type;
	std::string value;
}measurementData;

namespace Netapi {

class mysqlconnector {
public:
	mysqlconnector();
	virtual ~mysqlconnector();
	void insert(measurementData data);
	int start(int threads = 1);
	void insertWorker();
	bool finished(){return dbqueue.empty();}
private:

	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::SQLString add;
	std::queue<measurementData> dbqueue;
	std::mutex mutex;
	std::condition_variable cond;
	bool ready;
	//bool processed;

	void dbInsert(measurementData data);
};

} /* namespace Db */

#endif /* MYSQLCONNECTOR_H_ */
