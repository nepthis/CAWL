/*
 * mysqlconnector.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cawl-mobile
 */

#include "mysqlconnector.h"

namespace Db {

mysqlconnector::mysqlconnector() {
	res  		= NULL;
	stmt 		= NULL;
	add  		= "tcp://" + mysql_address + ":" + port;


	//Thread specific vars

	ready     	= false;
	//processed 	= false;

	std::thread worker(insertWorker);


	try{
		// Create a connection
		driver = get_driver_instance();
		con = driver->connect(add, user, password);

		// Connect to the cawl database
		con->setSchema(db);

	} catch (sql::SQLException &e) {
		// Throw something nice ;)
	}

	worker.join();
}

mysqlconnector::~mysqlconnector() {
	delete res;
	delete stmt;
	delete con;
}


void mysqlconnector::insertWorker(){
	//remove  from queue and insert into db

	while(true){
		cond.wait(lk);
		// sleep until condition
		while(ready){
			// Insert into DB or notify that queue is empty
			std::unique_lock<std::mutex> lk(mutex);

			if(!dbqueue.empty()){
				dbInsert(dbqueue.front());
				dbqueue.pop();

			}else{
				ready=false;
			}
		}
	}
}

void mysqlconnector::insert(measurementData data){
	//insert into queue, add mutex etc

	{
		std::unique_lock<std::mutex> lk(mutex);
		ready = false;
		dbqueue.push(data);
		cond.notify_all();
		ready = true;
	}
}

void mysqlconnector::dbInsert(measurementData data) {
	stmt = con->createStatement();
	name   =data.name;
	time   =data.timeStamp;
	type   =data.type;
	mdata  =data.value;
	cawlId =data.id;

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
