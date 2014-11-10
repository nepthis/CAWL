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
using namespace Ground_control;

typedef struct States{
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
int checkInput(int argc, char * args[], State * s){
	for(int i = 1; i < argc; i++){
		if ((((std::string)args[i] == "ground") ||((std::string)args[i] == "mobile") ) && (s->mode == "empty"))
		{s->mode = (std::string)args[i];continue;}
		if(((std::string)args[i] == "-i")){s->ip = true;continue;}
		if(checkIP(args[i])){s->ipAddr.push_back(args[i]);continue;}
		if(((std::string)args[i] == "-s")&&(not s->udp)){s->sctp = true;continue;}
		if(((std::string)args[i] == "-u")&&(not s->sctp)){s->udp = true;continue;}
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
		Major_Tom::Mobile *major = new Major_Tom::Mobile(s->sctp); //make into input args later
		while(retr){
			if(not major->em.socketsAreChecked()){	//I know, nested if is ugly but meh. I am lazy with this thing.
				if(not major->em.setUpSockets()){printf("Error setting up sockets...Exiting\n");exit(1);}
			}else
				if(not major->startUp()){ //The bool should be sctp variable in status.
					printf("Sending relay data failed, retrying in %i seconds\n", TIMEOUT);
					retr--;
					sleep(TIMEOUT);
					continue;
				}else{
					try{
						printf("Relaypackets sent\n");
						printf("Starting threads.\n");
						std::thread m1(&Major_Tom::Mobile::recvGround, major);
						std::thread m2(&Major_Tom::Mobile::sendEBUOne, major);
						std::thread m3(&Major_Tom::Mobile::sendEBUTwo, major);
						m1.join();
						m2.join();
						m3.join();
					}catch(int e){
						major->sendAllStop();
						major->pleased = true;
						perror("Main: Mobile");
						printf("Exiting\n");
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
		start(&s);
	}
	return 1;
}
