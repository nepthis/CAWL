/*
 * testClient.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-mobile
 */

#include <stdio.h>
#include <string>
#include "../Netapi/Host.h"
#include <utility>

using namespace Netapi;

int main(){
	Host host1 = Host("10.0.0.1", 10, true, "10.0.0.2");
	std::string boo;
	if(host1.getServer()){
		boo = "True";
	}else{
		boo = "False";
	}

	std::pair <std::string, std::string> addr = host1.getAddr();

	printf("Address1: %s , Address2: %s , Port: %i ,IsServer: %s \n", addr.first.c_str() , addr.second.c_str(),  host1.getPort(), boo.c_str());
	return 0;
}


