/*
 * mysqlconnector.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cawl-mobile
 */

#include "mysqlconnector.h"

namespace Db {

mysqlconnector::mysqlconnector() {
	res = NULL;
	stmt= NULL;
	add = "tcp://" + mysql_address + ":" + port;

	try{
		// Create a connection
		driver = get_driver_instance();
		con = driver->connect(add, user, password);

		// Connect to the cawl database
		con->setSchema(db);

	} catch (sql::SQLException &e) {
		  // Throw something nice ;)
	}
}

mysqlconnector::~mysqlconnector() {
	delete res;
	delete stmt;
	delete con;
}


void mysqlconnector::insertWorker(){
	//remove  from queue and insert into db
}

void mysqlconnector::insert(std::string data[5]){
	//insert into queue
}

void mysqlconnector::dbInsert(std::string data[5]) {
	stmt = con->createStatement();
	name   =data[0];
	time   =data[1];
	type   =data[2];
	mdata  =data[3];
	cawlId =data[4];

	// Form querey
	quer   ="INSERT INTO `" +
	        tbl				+
	        "` (`id`"		+
	        ", `name`"		+
	        ", `type`"		+
	        ", `time`"		+
	        ", `data`"		+
	        ",`cawlId`"		+
	        ") VALUES ("	+
	        "NULL, \'"		+
	        name+"\', \'"  	+
	        time+"\', \'"   +
	        type+"\', \'"   +
	        mdata+"\', \'"  +
	        cawlId+"\')";

	// Insert into database and catch exception if any is thrown
	try{
		stmt->execute(quer);
	} catch (sql::SQLException &e) {
		  // Throw something nice ;)
	}

}

} /* namespace Db */
