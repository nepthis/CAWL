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

#include <queue>


#define mysql_address 	"127.0.0.1"
#define port 			"3306"
#define user			"root"
#define password		"gurksallad"
#define db				"cawl"
#define tbl				"tests"



namespace Db {

class mysqlconnector {
public:
	mysqlconnector();
	virtual ~mysqlconnector();

	void mysqlconnector::insert(std::string data[5]);

private:
	sql::Driver *driver;
	sql::Connection *con;
	sql::Statement *stmt;
	sql::ResultSet *res;
	sql::SQLString add;

	std::string name;
	std::string time;
	std::string type;
	std::string mdata;
	std::string cawlId;
	std::string quer;

	std::queue<std::string[5]> dbqueue;

	void mysqlconnector::dbInsert(std::string data[5]);

	void mysqlconnector::insertWorker();
};

} /* namespace Db */

#endif /* MYSQLCONNECTOR_H_ */
