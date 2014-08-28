/*
 * main.cpp
 *
 *  Created on: Jun 19, 2014
 *      Author: Robin Bond
 */
#include <stdio.h> 		//Mainly for printf()
#include <signal.h>	//Used for catching ctr+c
#include <string>		//Standard string, used for addresses mostly
#include <thread>		//for standard threads, mutexes exist in objects
#include <mutex>
#include <chrono>

#include "Ground_control/Ground.h"
#include "Major_Tom/Mobile.h"

#define RETRIES 5 		//Amount of tries for Mobile to start connection to server
#define TIMEOUT 5	//Amount of seconds to wait before retrying
void INT_handler(int dummy){
	//printf("Shutting Down...\n");
	exit(EXIT_SUCCESS);
}
using namespace std;

int main(int argc, char * args[]){
	signal(SIGINT, INT_handler);	//When exiting with ctrl+c
	int rt = RETRIES;
	for(int i = 0; i < argc; i++){
		if ((std::string)args[1] == "ground"){
			continue;
		}else if((std::string)args[1] == "mobile"){
			continue;
		}else{
			printf("arguments provided: %s\n", args[1]);
			printf("Arguments provided are invalid.\nTry <sudo ./cawl mobile\n");
			exit(0);
		}
	}
	if((std::string)args[1] == "ground"){
		try{
			Ground* gc =  new  Ground(); //(char*)"192.168.2.5",(char*) "192.168.2.100"
			std::thread g1(&Ground::startSend, gc);
			printf("Main: thread started, joining\n");
			g1.join();
			printf("Finished.\n");
			exit(1);
		}catch(int e){
			printf("Error number: %i\n", e);
			perror("Description: ");
			exit(0);
		}


	}
	else if((std::string)args[1] =="mobile"){
		Major_Tom::Mobile *major = new Major_Tom::Mobile(); //make into input args later
		while(rt){
			try{
				printf("Starting threads.\n");
				std::thread m1(&Major_Tom::Mobile::socketReceive, major);
				std::thread m2(&Major_Tom::Mobile::ebuSend, major);
				m1.join();
				m2.join();
				printf("Finished.\n");
				exit(1);
			}catch(int e){
				major->pleased = true;
				printf("Error number: %i\n", e);
				perror("Description: ");
				exit(0);
			}
			sleep(TIMEOUT);
		}


	}
	printf("Main will terminate\n");
	exit(0);
}
