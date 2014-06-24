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

#include "../Netapi/MeasurementData.h"

#define mysql_address 	"127.0.0.1"
#define port 			"3306"
#define user			"cawl"
#define password		"gurksallad"
#define db				"cawl"
#define tbl				"tests"

namespace Db {

class mysqlconnector {
public:
	mysqlconnector();
	virtual ~mysqlconnector();
	void insert(measurementData data);
	void start(int threads = 1);
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
