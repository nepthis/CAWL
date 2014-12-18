/*
 * mysqlconnector.cpp
 *
 *  Created on: Jun 17, 2014
 *      Author: cawl-mobile
 */

#include "mysqlconnector.h"

namespace Netapi {

mysqlconnector::mysqlconnector() {

	res         = NULL;
	stmt        = NULL;
	add         = (std::string)("tcp://") + mysql_address + (std::string)(":") + port;

	con         = NULL;
	driver      = NULL;

	//Thread specific vars
	ready       = false;
	//processed   = false;

}

void mysqlconnector::createTable(){
	stmt = con->createStatement();
	// Form querey
	std::string quer;
	quer = (std::string)"DROP TABLE IF EXISTS `"+tbl+(std::string)"`;";
	try{
		res = stmt->executeQuery(quer);
	}catch (sql::SQLException &e) {
		// log this shit, dont print!!!!!
			///  vvvvvvvvv \\\
		printf("%s\n",e.what());
	}

	quer = (std::string)"CREATE TABLE "+tbl+(std::string)" (id INT NOT NULL"+
		 			  (std::string)" AUTO_INCREMENT PRIMARY KEY,timestamp VARCHAR(32)"+
					  (std::string)", sstat_assoc_id VARCHAR(32)"+
                      (std::string)", sstat_state VARCHAR(32)"+
                      (std::string)", sstat_rwnd VARCHAR(32)"+
                      (std::string)", sstat_unackdata VARCHAR(32)"+
                      (std::string)", sstat_penddata VARCHAR(32)"+
                      (std::string)", sstat_instrms VARCHAR(32)"+
                      (std::string)", sstat_outstrms VARCHAR(32)"+
                      (std::string)", sstat_fragmentation_point VARCHAR(32)"+
                      (std::string)", spinfo_address VARCHAR(32)"+
                      (std::string)", spinfo_state VARCHAR(32)"+
                      (std::string)", spinfo_cwnd VARCHAR(32)"+
                      (std::string)", spinfo_srtt VARCHAR(32)"+
                      (std::string)", spinfo_rto VARCHAR(32)"+
                      (std::string)", spinfo_mtu VARCHAR(32));";
	try{
		res = stmt->executeQuery(quer);
	}catch (sql::SQLException &e) {
		// log this shit, dont print!!!!!
			///  vvvvvvvvv \\\
		printf("%s\n",e.what());
	}
}


void mysqlconnector::tableCheck(){

	stmt = con->createStatement();
	// Form querey
	std::string quer;
	int rows;

	// Forms query for checking if table exist
	quer = (std::string)"SELECT count(*) FROM information_schema.TABLES WHERE"+
			(std::string)"(TABLE_SCHEMA = \'" +
			db +
			(std::string)"\') AND (TABLE_NAME = \'" +
			tbl +
			(std::string)"\');";

	try{
		res = stmt->executeQuery(quer);
		res->next();
		rows = std::stoi(res->getString(1));
		printf("rows: %i \n",rows);
	}catch (sql::SQLException &e){
		createTable();
		// log this shit, dont print!!!!!
		///  vvvvvvvvv \\\
		printf("%s\n",e.what());
	}

	// Forms query to check if columns are named correct
	quer =(std::string)"SELECT "+
			"`sstat_assoc_id`,"+
			"`sstat_state`,"+
			"`sstat_rwnd`,"+
			"`sstat_unackdata`,"+
			"`sstat_penddata`,"+
			"`sstat_instrms`,"+
			"`sstat_outstrms`,"+
			"`sstat_fragmentation_point`,"+

			"`spinfo_address`,"+
			"`spinfo_state`,"+
			"`spinfo_cwnd`,"+
			"`spinfo_srtt`,"+
			"`spinfo_rto`,"+
			"`spinfo_mtu`"+
			" FROM `" +
			tbl +
			(std::string)"` LIMIT 1;"
			;
	if(rows > 0){
		try{
			res = stmt->executeQuery(quer);
			int rows = res->rowsCount();
			printf("Rows: %i\n", rows);
		}catch (sql::SQLException &e) {
			createTable();
			// log this shit, dont print!!!!!
			///  vvvvvvvvv \\\
			printf("%s\n",e.what());
		}
	}else{
		createTable();
	}
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


int mysqlconnector::start(int threads){

	try{
		// Create a connection
		driver = get_driver_instance();
		con = driver->connect(add, user, password);

		/* Connect to the cawl database, should check
		*if db exist and create if it does not
		*/
		con->setSchema(db);

		// Check if table exist
		tableCheck();

	} catch (sql::SQLException &e) {
		return -1;
	}

	// Min 1, Max 8 workerthreads
	(threads<1) ? threads=1 : (threads<=8) ? threads : threads=8;

	for(int i=0;i<threads;i++){
		std::thread worker(&mysqlconnector::insertWorker, this);
		worker.detach();
	}
	return 1;
}


void mysqlconnector::insert(stat data){
	//insert into queue, add mutex etc
	{
		std::unique_lock<std::mutex> lk(mutex);
		ready = false;
		dbqueue.push(data);
		cond.notify_all();
		ready = true;
	}
}


void mysqlconnector::dbInsert(stat data) {

	stmt = con->createStatement();

	// Form querey
	std::string quer   =(std::string)("INSERT INTO `") +
			tbl	+
			(std::string)(", `sstat_assoc_id`")		+
			(std::string)(", `sstat_state`")		+
			(std::string)(", `sstat_unackdata`")	+
			(std::string)(", `spinfo_address`")		+
			(std::string)(", `spinfo_srtt`")		+
			(std::string)(") VALUES (")		+
			(std::string)("NULL, \'")		+
			data.sstat_assoc_id+(std::string)("\', \'")	+
			data.sstat_state+(std::string)("\', \'")    +
			data.sstat_unackdata+(std::string)("\', \'")   +
			data.spinfo_address+(std::string)("\', \'")   +
			data.spinfo_srtt+(std::string)("\')");

	// Insert into database and catch exception if any is thrown
	try{
		stmt->execute(quer);
	} catch (sql::SQLException &e){
		printf("%s\n",e.what());
		throw 20;
	}
}


mysqlconnector::~mysqlconnector() {
	delete res;
	delete stmt;
	delete con;
}

} /* namespace Db */
