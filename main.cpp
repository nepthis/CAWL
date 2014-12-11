/*
 * main.cpp
 *  Created on: Jun 19, 2014
 *  Author: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */
#include <stdio.h> 		//printf()
#include <signal.h>	//Used for catching ctr+c
#include <string>	//Standard string, used for IP addresses mostly
#include <thread>
#include <regex>
#include <chrono>
#include <arpa/inet.h>
#include <vector>
#include <errno.h>
#include "Ground_control/Ground.h"
#include "Major_Tom/Mobile.h"
#include "logger.h"
#include "Globals.h"

#define RETRIES 5 		//Amount of retries
#define TIMEOUT 5	//Amount of seconds to wait before retrying
#define connected true
sig_atomic_t signaled = 0;
/* This
 */


void exit_handler(int dummy){
	signaled = 1;
}
using namespace std;

/* Stores the users input
 */
typedef struct States{
	bool ip = false;
	bool sctp, udp, changeLoggingLevel = false;
	std::string mode = "empty";
	std::vector<std::string>  ipAddr;
	std::string loggingLevel = "error";
}State;

/* Validates IPv4 addresses
 */

bool checkIP(char *ipAddress){
	struct sockaddr_in temp;
	if( inet_pton(AF_INET, ipAddress, &(temp.sin_addr))){return true;}
	else{return false;}
}
std::string lowercase(std::string input){
	std::string output;

	return output;
}
/*
 *
 */
int checkInput(int argc, char * args[], State * s){
	std::locale loc;
	for(int i = 1; i < argc; i++){
		if ((((std::string)args[i] == "ground") ||((std::string)args[i] == "mobile") ) && (s->mode == "empty")){
			s->mode = (std::string)args[i];continue;
		}
		if(((std::string)args[i] == "-i")){
			s->ip = true;continue;
		}
		if(checkIP(args[i])){s->ipAddr.push_back(args[i]);continue;}
		if(((std::string)args[i] == "-s")&&(not s->udp)){
			s->sctp = true;continue;
		}
		if(((std::string)args[i] == "-u")&&(not s->sctp)){
			s->udp = true;continue;
		}
		if((std::string)args[i] == "-l"){
			s->changeLoggingLevel = true;continue;
		}
		if ((((std::string)args[i] == "warning") ||((std::string)args[i] == "info")||((std::string)args[i] == "verbose") ) && (s->changeLoggingLevel)){
			s->loggingLevel = (std::string)args[i];continue;
		}

	}
	if(s->ip && s->ipAddr.empty()){return -1;}
	else if(not s->ipAddr.empty()){
		printf("Amount of valid IP addresses found: %i\nCurrent mode: %s\n", s->ipAddr.size(), s->mode.c_str());}
	return 0;
}
/* Based on the state set from checkInput the start function creates and runs either
 * Mobile or Ground. It terminates if an error occurs.
 * If one of the fucntions for setting up the sockets spits out an error it will
 * try again until the specified amount is reached.
 */
void start(State * s){
	int retr = RETRIES;
	int rtGround = RETRIES;
	if(s->mode == "ground"){
		Ground_control::Ground* gc =  new  Ground_control::Ground(s->sctp); //(char*)"192.168.2.5",(char*) "192.168.2.100"
		while(retr){
			if (gc->simulator->connectToSim() != connected){
				printf("Socket for simulator failed, retrying in %i seconds\n", TIMEOUT);
				retr--;
				sleep(TIMEOUT);
			}else{
				try{
					//needs more threads...IMU receive
					logVerbose("Main: thread starting\n");
					std::thread g1(&Ground_control::Ground::sendMobile, gc);	//For simulator data to Mobile
					std::thread g2(&Ground_control::Ground::receiveSim, gc);	//For receiving data from simulator
					std::thread g3(&Ground_control::Ground::receiveImuPacket, gc);
					std::thread g4(&Ground_control::Ground::sendSim, gc);
					logVerbose("Main: thread started, joining\n");
					g1.join();
					g2.join();
					g3.join();
					g4.join();
				}catch(int e){
					logError(strerror(errno));
					break;
				}
			}
		}
	}
	if(s->mode=="mobile"){
		Major_Tom::Mobile *major = new Major_Tom::Mobile(s->sctp); //make into input args later
		while(retr){
			if(not major->em.socketsAreChecked()){	//I know, nested if is ugly but meh. I am lazy with this thing.
				if(not major->em.setUpSockets()){logError("Can not set up sockets: Exiting");exit(1);}
			}else
				if(not major->startUp()){ //The bool should be sctp variable in status.
					printf("Sending relay data failed, retrying in %i seconds\n", TIMEOUT);
					logWarning("Can not send relay packages");
					retr--;
					sleep(TIMEOUT);
					continue;
				}else{
					try{
						logVerbose("Relay packages sent");
						logVerbose("Starting threads");
						std::thread m1(&Major_Tom::Mobile::recvGround, major);
						std::thread m2(&Major_Tom::Mobile::sendEBUOne, major);
						std::thread m3(&Major_Tom::Mobile::sendEBUTwo, major);
						std::thread m4(&Major_Tom::Mobile::recvIMU, major);
						m1.join();
						m2.join();
						m3.join();
						m4.join();
					}catch(int e){
						major->sendAllStop();
						signaled = 1;
						logError(strerror(errno));
						break;
					}
				}
		}
	}
}

int main(int argc, char * args[]){

	signal(SIGINT, exit_handler);	//When exiting with ctrl+c
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
