/*
 * inputParser.cpp
 *
 *  Created on: Sep 19, 2014
 *      Author: Robin Bond
 */

#include <stdint.h>
#include <string>
#include <cstdio>
#include <unistd.h>
#include <iostream>
#include <arpa/inet.h>


using namespace std;
bool checkIP(char *ipAddress){
	struct sockaddr_in temp;
	if( inet_pton(AF_INET, ipAddress, &(temp.sin_addr))){
		return true;
	}else{
		return false;
	}
}

int main(int argc, char * args[]){
	for (int i = 0; i < argc; i++){
		if (checkIP(args[i])){
			printf("yay\n");
		}
	}
}




