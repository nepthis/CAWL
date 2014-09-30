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
#include <regex>
#include <chrono>
#include <arpa/inet.h>
#include <vector>

#include "Ground_control/Ground.h"
#include "Major_Tom/Mobile.h"

#define RETRIES 5 		//Amount of retries
#define TIMEOUT 5	//Amount of seconds to wait before retrying
#define connected true

void INT_handler(int dummy){
	exit(EXIT_SUCCESS);
}
using namespace std;

typedef struct state{
	bool ip = false;
	bool sctp, udp = false;
	std::string mode = "empty";
	std::vector<std::string>  ipAddr;
}State;
bool checkIP(char *ipAddress){
	struct sockaddr_in temp;
	if( inet_pton(AF_INET, ipAddress, &(temp.sin_addr))){return true;}
	else{return false;}
}
int setInput(int argc, char * args[], State * s){
	for(int i = 1; i < argc; i++){
		if ((((std::string)args[i] == "ground") ||((std::string)args[i] == "mobile") ) && (s->mode == "empty"))
		{s->mode = (std::string)args[i];continue;}
		if(((std::string)args[i] == "-i")){s->ip = true;continue;}
		if(checkIP(args[i])){s->ipAddr.push_back(args[i]);continue;}
		if(((std::string)args[i] == "-s")&&(not s->udp)){s->sctp = true;continue;}
		if(((std::string)args[i] == "-u")&&(not s->sctp)){s->udp = true;continue;}
	}
	if(s->ip && s->ipAddr.empty()){return -1;}
	else if(not s->ipAddr.empty())
	{printf("Amount of valid IP addresses found: %i\nCurrent mode: %s\n", s->ipAddr.size(), s->mode.c_str());}
	return 0;
}
void startUp(State * s){
	int rtMobile = RETRIES;
	int rtGround = RETRIES;
	if(s->mode == "ground"){
		Ground* gc =  new  Ground(); //(char*)"192.168.2.5",(char*) "192.168.2.100"
		while(rtGround){
			if (gc->simulator->connectToSim() != connected){
				printf("Socket for simulator failed, retrying in %i seconds\n", TIMEOUT);
				rtGround--;
				sleep(TIMEOUT);
			}else{
				try{
					//needs more threads...IMU receive
					std::thread g1(&Ground::startSend, gc);	//For simulator data to Mobile
					std::thread g2(&Ground::startRecieve, gc);	//For receiving data from simulator
					printf("Main: thread started, joining\n");
					g1.join();
					g2.join();
				}catch(int e){
					printf("Error number: %i\n", e);
					perror("Description: ");
					exit(1);
				}
			}
		}
	}
	if(s->mode=="mobile"){
		Major_Tom::Mobile *major = new Major_Tom::Mobile(); //make into input args later
		while(rtMobile){
			if(not major->em.connectToEBU()){
				printf("Connecting to EBUs failed, retrying in %i seconds\n", TIMEOUT);
				rtMobile--;
				sleep(TIMEOUT);
			}else{
				try{
					major->em.recAnalogIn();
					major->em.recDigitalIn();
					major->em.sendRelayCommand(major->rPackOne, 1);
					major->em.sendRelayCommand(major->rPackTwo, 2);
					printf("Starting threads.\n");
					std::thread m1(&Major_Tom::Mobile::socketReceive, major);
					std::thread m2(&Major_Tom::Mobile::ebuOneSend, major);
					//std::thread m3(&Major_Tom::Mobile::ebuTwoSend, major);
					m1.join();
					m2.join();
					//m3.join();
				}catch(int e){
					major->pleased = true;
					printf("Error number: %i\n", e);
					perror("Description: ");
					exit(-1);
				}
			}
		}
	}
}
int main(int argc, char * args[]){
	signal(SIGINT, INT_handler);	//When exiting with ctrl+c
	State s;
	if(not (setInput(argc, args,& s) == 0)){
		printf("ERROR parsing input\n");
		exit(1);
	}
	startUp(&s);

	return 1;
}
