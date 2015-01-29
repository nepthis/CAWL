/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
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




