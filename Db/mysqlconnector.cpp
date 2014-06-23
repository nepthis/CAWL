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
	add  		= (std::string)("tcp://") + mysql_address + (std::string)(":") + port;

	con = NULL;
	driver = NULL;

	//Thread specific vars

	ready     	= false;
	//processed 	= false;

}


mysqlconnector::~mysqlconnector() {
	delete res;
	delete stmt;
	delete con;
}


void mysqlconnector::insertWorker(){
	//remove  from queue and insert into db

	while(true){
		std::unique_lock<std::mutex> lk(mutex);
		cond.wait(lk);

		// sleep until condition
		while(ready){
			// Insert into DB or notify that queue is empty

			if(!dbqueue.empty()){
				dbInsert(dbqueue.front());
				dbqueue.pop();

			}else{
				ready=false;
			}
		}
	}
}

void mysqlconnector::start(int threads){
	try{
		// Create a connection
		driver = get_driver_instance();
		con = driver->connect(add, user, password);

		// Connect to the cawl database
		con->setSchema(db);

	} catch (sql::SQLException &e) {
		// Throw something nice ;)
	}

	// Min 1, Max 8 workerthreads
	(threads<1) ? threads=1 : (threads<=8) ? threads : threads=8;

	for(int i=0;i<threads;i++){
		std::thread worker(&mysqlconnector::insertWorker, this);
		worker.detach();
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
	std::string name   =data.name;
	std::string dtime  =data.timeStamp;
	std::string type   =data.type;
	std::string mdata  =data.value;
	std::string cawlId =data.id;

	// Form querey
	std::string quer   =(std::string)("INSERT INTO `") +
			tbl				+
			(std::string)("` (`id`")		+
			(std::string)(", `name`")		+
			(std::string)(", `type`")		+
			(std::string)(", `time`")		+
			(std::string)(", `data`")		+
			(std::string)(",`cawlId`")		+
			(std::string)(") VALUES (")		+
			(std::string)("NULL, \'")		+
			name+(std::string)("\', \'")	+
			dtime+(std::string)("\', \'")   +
			type+(std::string)("\', \'")    +
			mdata+(std::string)("\', \'")   +
			cawlId+(std::string)("\')");

	// Insert into database and catch exception if any is thrown
	try{
		stmt->execute(quer);
	} catch (sql::SQLException &e) {
		// Throw something nice ;)
	}
}

} /* namespace Db */
