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

#define RETRIES 5 		//Amount of retries
#define TIMEOUT 5	//Amount of seconds to wait before retrying
#define connected true

void INT_handler(int dummy){
	exit(EXIT_SUCCESS);
}
using namespace std;

int main(int argc, char * args[]){
	signal(SIGINT, INT_handler);	//When exiting with ctrl+c
	int rtMobile = RETRIES;
	int rtGround = RETRIES;
	for(int i = 0; i < argc; i++){
		if ((std::string)args[1] == "ground"){
			continue;
		}else if((std::string)args[1] == "mobile"){
			continue;
		}else{
			printf("arguments provided: %s\n", args[1]);
			printf("Arguments provided are invalid.\nTry <sudo ./cawl mobile\n");
			exit(-1);
		}
	}
	if((std::string)args[1] == "ground"){
		Ground* gc =  new  Ground(); //(char*)"192.168.2.5",(char*) "192.168.2.100"
		while(rtGround){
			if (gc->simulator->connectToSim() != connected){
				printf("Socket for simulator failed, retrying in %i seconds\n", TIMEOUT);
				rtGround--;
				sleep(TIMEOUT);
			}else{
				try{
					//needs more threads...
					std::thread g1(&Ground::startSend, gc);	//For simulator data to Mobile
					std::thread g2(&Ground::startRecieve, gc);	//For receiving data from simulator
					printf("Main: thread started, joining\n");
					g1.join();
					g2.join();
					printf("Finished.\n");
					exit(0);
				}catch(int e){
					printf("Error number: %i\n", e);
					perror("Description: ");
					exit(-1);
				}
			}
		}
	}
	else if((std::string)args[1] =="mobile"){
		Major_Tom::Mobile *major = new Major_Tom::Mobile(); //make into input args later
		while(rtMobile){
			if(not major->em.connectToEBU()){
				printf("Connecting to EBUs failed, retrying in %i seconds\n", TIMEOUT);
				rtMobile--;
				sleep(TIMEOUT);
			}else{
				try{
					major->em.sendRelayCommand(major->rPackOne, 1);
					major->em.sendRelayCommand(major->rPackTwo, 2);
					printf("Starting threads.\n");
					std::thread m1(&Major_Tom::Mobile::socketReceive, major);
					std::thread m2(&Major_Tom::Mobile::ebuSend, major);
					m1.join();
					m2.join();
					printf("Finished.\n");
					exit(0);
				}catch(int e){
					major->pleased = true;
					printf("Error number: %i\n", e);
					perror("Description: ");
					exit(-1);
				}
			}
		}
	}
	printf("Main will terminate\n");
	exit(0);
}
