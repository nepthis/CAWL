/*
 * testClient.cpp
 *
 *  Created on: Apr 16, 2014
 *      Author: cawl-mobile
 */

#include <stdio.h>

using namespace std;

int main(){
	Host host1 = Host(*"test",10, false);
	printf("Ip: %i \n", host1.GetPort());
	return 0;
}


