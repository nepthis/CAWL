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
#include <errno.h>

#include "Ground_control/Ground.h"
#include "Major_Tom/Mobile.h"
#include "logger.h"

#define RETRIES 5 		//Amount of retries
#define TIMEOUT 5	//Amount of seconds to wait before retrying
#define connected true

void INT_handler(int dummy){
	exit(EXIT_SUCCESS);
}
using namespace std;
using namespace Ground_control;

typedef struct States{
	bool ip = false;
	bool sctp, udp = false;
	std::string mode = "empty";
	std::vector<std::string>  ipAddr;
	std::string loggingLevel = "error";
}State;

bool checkIP(char *ipAddress){
	struct sockaddr_in temp;
	if( inet_pton(AF_INET, ipAddress, &(temp.sin_addr))){return true;}
	else{return false;}
}
int checkInput(int argc, char * args[], State * s){
	for(int i = 1; i < argc; i++){
		if ((((std::string)args[i] == "ground") ||((std::string)args[i] == "mobile") ) && (s->mode == "empty"))
		{s->mode = (std::string)args[i];continue;}
		if(((std::string)args[i] == "-i")){s->ip = true;continue;}
		if(checkIP(args[i])){s->ipAddr.push_back(args[i]);continue;}
		if(((std::string)args[i] == "-s")&&(not s->udp)){s->sctp = true;continue;}
		if(((std::string)args[i] == "-u")&&(not s->sctp)){s->udp = true;continue;}
		if((std::string)args[i] == "-l"){s->udp = true;continue;}
		if ((((std::string)args[i] == "warning") ||((std::string)args[i] == "info")||((std::string)args[i] == "verbose") ) && (s->loggingLevel == "error"))
		{s->loggingLevel = (std::string)args[i];continue;}

	}
	if(s->ip && s->ipAddr.empty()){return -1;}
	else if(not s->ipAddr.empty()){
		printf("Amount of valid IP addresses found: %i\nCurrent mode: %s\n", s->ipAddr.size(), s->mode.c_str());}
	return 0;
}
void start(State * s){
	int retr = RETRIES;
	int rtGround = RETRIES;
	if(s->mode == "ground"){
		Ground* gc =  new  Ground(s->sctp); //(char*)"192.168.2.5",(char*) "192.168.2.100"
		while(retr){
			if (gc->simulator->connectToSim() != connected){
				printf("Socket for simulator failed, retrying in %i seconds\n", TIMEOUT);
				retr--;
				sleep(TIMEOUT);
			}else{
				try{
					//needs more threads...IMU receive
					printf("Main: thread starting\n");
					std::thread g1(&Ground::sendMobile, gc);	//For simulator data to Mobile
					std::thread g2(&Ground::receiveSim, gc);	//For receiving data from simulator
					//std::thread g3(&Ground::receiveImuPacket, gc);
					printf("Main: thread started, joining\n");
					g1.join();
					g2.join();
					//g3.join();
					printf("Main: thread started, joined\n");
				}catch(int e){
					logError(strerror(errno));
					exit(1);
				}
			}
		}
	}
	if(s->mode=="mobile"){
		printf("boot\n");
		Major_Tom::Mobile *major = new Major_Tom::Mobile(s->sctp); //make into input args later
		while(retr){
			if(not major->em.socketsAreChecked()){	//I know, nested if is ugly but meh. I am lazy with this thing.
				printf("socketcheck\n");
				if(not major->em.setUpSockets()){logError("Can not set up sockets: Exiting");exit(1);}
				printf("sockets are setup\n");
			}else
				printf("\nafter boot\n");
				if(not major->startUp()){ //The bool should be sctp variable in status.
					printf("Sending relay data failed, retrying in %i seconds\n", TIMEOUT);
					logWarning("Can not send relay packages");
					retr--;
					sleep(TIMEOUT);
					continue;
				}else{
					printf("mobile thing\n");
					try{
						logVerbose("Relay packages sent");
						logVerbose("Starting threads");
						printf("Starting Threads\n");
						std::thread m1(&Major_Tom::Mobile::recvGround, major);
						std::thread m2(&Major_Tom::Mobile::sendEBUOne, major);
						std::thread m3(&Major_Tom::Mobile::sendEBUTwo, major);
						//std::thread m4(&Major_Tom::Mobile::recvIMU, major);
						printf("Joining Threads\n");
						m1.join();
						m2.join();
						m3.join();
						//m4.join();
						printf("Threads joined\n");
					}catch(int e){
						major->sendAllStop();
						major->pleased = true;
						logError(strerror(errno));
						exit(-1);
					}
				}
		}
	}
}
int main(int argc, char * args[]){
	signal(SIGINT, INT_handler);	//When exiting with ctrl+c
	State s;
	if(not (checkInput(argc, args,& s) == 0)){
		printf("ERROR parsing input\n");
		exit(1);
	}else{
		logLevel = s.loggingLevel;
		start(&s);
	}
	return 1;
}
