/*
 * mysqlconnector.h
 *
 *  Created on: Jun 17, 2014
 *      Author: cawl-mobile
 */

#ifndef MYSQLCONNECTOR_H_
#define MYSQLCONNECTOR_H_

// Used for MYSQL connection
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

// Used for threading
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <stdio.h>

// Database credentials etc.
#define mysql_address   "127.0.0.1"
#define port            "3306"
#define user            "cawl"
#define password        "ultrasupergurksallad"
#define db              "cawl"
#define tbl             "tests"

namespace Netapi {

typedef struct stat{
	int sstat_assoc_id;
	int sstat_state;
	int sstat_rwnd;
	int sstat_unackdata;
	int sstat_penddata;
	int sstat_instrms;
	int sstat_outstrms;
	int sstat_fragmentation_point;

	std::string spinfo_address;
	int spinfo_state;
	int spinfo_cwnd;
	int spinfo_srtt;
	int spinfo_rto;
	int spinfo_mtu;
}stat;

class mysqlconnector {
public:
	mysqlconnector();
	virtual ~mysqlconnector();
	void insert(stat data);
	void tableCheck();
	int start(int threads = 1);
	void insertWorker();
	bool finished(){return dbqueue.empty();}
private:
	void createTable();
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::SQLString add;
	std::queue<stat> dbqueue;
	std::mutex mutex;
	std::condition_variable cond;
	bool ready;
	//bool processed;

	void dbInsert(stat data);
};

} /* namespace Db */

#endif /* MYSQLCONNECTOR_H_ */
