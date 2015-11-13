/*
 * DbTest.cpp
 *
 *  Created on: Jun 18, 2014
 *      Author: cawl-mobile
 */


#include <stdio.h>
#include <unistd.h> //for sleep
#include <cstring>
#include <iostream>
#include "../Db/mysqlconnector.h"
#include "../Netapi/MeasurementData.h"
#include <sstream>
#include <string>

using namespace std;

int main(){
	Db::mysqlconnector *sql = new Db::mysqlconnector();
	measurementData data = measurementData{"5","Test1","140618220001","RTT","1234567890"};
	measurementData data2 = measurementData{"4","Test4","140620220001","GPUT","0987654321"};
	measurementData data3 = measurementData{"5","Test3","150618221201","RTT","1111111111"};

	sql->start(1);
	int i = 0;
	/*while(i<100){
		sql->insert(data2);
		sql->insert(data3);
		sql->insert(data2);
		sql->insert(data3);
		sql->insert(data2);
		sql->insert(data3);
		i++;

	}*/
	//sleeptest
	sql->insert(data2);
	sleep(5);
	sql->insert(data2);

	while(!sql->finished());
}
