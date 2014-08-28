#include <curses.h>
#include <string>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

#include "../Packets/SimPack.h"

#define MAX 4.5
#define MIN 0.5
#define FREQ 10
#define PORT 65400
#define ADDR "192.168.2.100"


#define OFF 0
#define ON 1

std::mutex ml;

void simSend(Packets::SimPack* spack){

	socklen_t slen = sizeof(struct sockaddr_in);
	struct sockaddr_in addr;
	int sock;

	if((sock = socket(AF_INET,SOCK_DGRAM,0)) < 0){
		perror("socket error");
		// extend
		endwin();
		exit(0);
	}

	memset((char *)&addr, 0, sizeof(addr));
	inet_pton(AF_INET, ADDR, &(addr.sin_addr));
	addr.sin_port = htons(PORT);

	while(1){
		ml.lock();
		try{
			sendto(sock, (char*)&spack->fromSim, sizeof(spack->fromSim), 0, (struct sockaddr*) &addr, slen);
		}catch(int e){
			//extend
			endwin();
			exit(0);
		}
		ml.unlock();
		usleep(1000000/FREQ);
		}

}

const char* instructions(){
	std::string tmp;

	tmp =   "\n\n\n"
			"    Use arrow keys to change steering and throttle.\n"
			"    Throttle can only be changed when breaking is off!\n"
			"\n"
			"    Press keys \"N\" for neutral, \"F\" for forward or \"R\"\n"
			"    for reverse gear. This can only be changed when breaking\n"
			"    is applied!\n"
			"\n"
			"    Breaking is applied with the spacebar and automatically\n"
			"    sets throttle to zero! To remove breaking press the\n"
			"    \"B\" key.\n"
			"\n"
			"    To get to this help screen press the \"H\" key\n";

	return tmp.c_str();
}

void update(int i, float& vdir, float& brks, float& spd, int& forward, int& reverse, int& park, Packets::SimPack& spack){
	if(i == 110 && brks>0 && spd < 1){
		forward = 0;
		reverse = 0;

	}else if(i == 114 && brks>0 && spd < 1){
		forward = 0;
		reverse = 1;

	}else if(i == 102 && brks>0 && spd < 1){
		forward = 1;
		reverse = 0;

	}else if(i == 32){
		((brks < MAX) ? brks+=0.5 :brks);
		spd=MIN;

	}else if(i == 98){
		((brks > MIN) ? brks-=0.5 :brks);

	}else if(i == 67){
		((vdir < MAX-0.1) ? vdir+=0.1 :vdir);

	}else if(i == 68){
		((vdir > MIN+0.1) ? vdir-=0.1 :vdir);

	}else if(i == 66){
		((spd > MIN && brks < 1) ? spd-=0.1 :spd);

	}else if(i == 65){
		((spd < MAX-0.1 && brks < 1) ? spd+=0.1 :spd);
	}else if(i == 112 && brks>4.0){
		(park?park=0:park=1);
	}
	spack.setDigital(GEARCLCREVERSE,reverse);
	spack.setDigital(GEARCLCFORWARD,forward);
	spack.setDigital(PARKINGBRAKE,park);
	spack.setAnalog(GASPEDAL,spack.fiveToOne(spd));
	spack.setAnalog(BRAKEPEDAL,spack.fiveToOne(brks));
	spack.setAnalog(JOYSTICK,spack.fiveToOneNeg(vdir));
}


const char* txt(float vdir, float spd, float brks, int forward, int reverse, int parking){
	std::string v = std::to_string(vdir);
	std::string s = std::to_string(fabs(spd));
	std::string b = std::to_string(brks);
	std::string g;
	if(forward == 1){
		g="F";
	}else if(reverse == 1){
		g="R";
	}else{
		g="N";
	}
	std::string tmp = "\n\n"
			"    Stearing at voltage(0-5v):"+v+"\n"
			"    Throttle at voltage(0-5v):"+s+"\n"
			"    Breaks   at voltage(0-5v):"+b+"\n"
			"\n"
			"    Gear: "+g+" Parkingbrake: "+(parking?"ON":"OFF")+
			"\n\n\n"
			"    Press \"H\" key for help or \"Q\" to quit\n";

	return tmp.c_str();
}

const char* gui(float vdir, float spd, float brks, int forward, int reverse){
	std::string tmp;
	std::string v = std::to_string(vdir);
	std::string s = std::to_string(spd);
	std::string b = std::to_string(brks);
	std::string g;

	if(forward == 1){
		g="F";
	}else if(reverse == 1){
		g="R";
	}else{
		g="N";
	}

	if(vdir < 2.55 && vdir > 2.45){
		tmp = "\n\n\n    Direction          Gas           Breaks       Gear\n"
				"\n"
				"     |----|           | "+(std::string)((spd >= 4.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 4.0) ? "x" : " ")+" |\n"
				"      |  |            | "+(std::string)((spd >= 3.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 3.0) ? "x" : " ")+" |         "+g+"\n"
				"      |  |            | "+(std::string)((spd >= 2.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 2.0) ? "x" : " ")+" |\n"
				"     |----|           | "+(std::string)((spd >= 1.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 1.0) ? "x" : " ")+" |";
	}else if(vdir < 2.5){

		tmp= "\n\n\n    Direction          Gas           Breaks       Gear\n"
				"\n"
				"     \\----\\           | "+(std::string)((spd >= 4.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 4.0) ? "x" : " ")+" |\n"
				"      |  |            | "+(std::string)((spd >= 3.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 3.0) ? "x" : " ")+" |         "+g+"\n"
				"      |  |            | "+(std::string)((spd >= 2.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 2.0) ? "x" : " ")+" |\n"
				"     |----|           | "+(std::string)((spd >= 1.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 1.0) ? "x" : " ")+" |";
	}else if(vdir > 2.5){
		tmp = "\n\n\n    Direction          Gas           Breaks       Gear\n"
				"\n"
				"     /----/           | "+(std::string)((spd >= 4.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 4.0) ? "x" : " ")+" |\n"
				"      |  |            | "+(std::string)((spd >= 3.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 3.0) ? "x" : " ")+" |         "+g+"\n"
				"      |  |            | "+(std::string)((spd >= 2.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 2.0) ? "x" : " ")+" |\n"
				"     |----|           | "+(std::string)((spd >= 1.0) ? "x" : " ")+" |          | "+(std::string)((brks >= 1.0) ? "x" : " ")+" |";
	}
	return tmp.c_str();
}


int main()
{
	Packets::SimPack* spack =  new Packets::SimPack();

	float str = 2.5;
	float brk = 4.5;
	float gas = 0.5;
	int forward = 0;
	int reverse = 0;
	int parking = 1;
	int tmp;


	ml.lock();
	spack->setDigital(GEARSELECT1, ON);
	spack->setDigital(ACTIVATIONCLC,ON);
	ml.unlock();

	std::thread worker(simSend, spack);
	worker.detach();

	initscr();
	(void)noecho();
	curs_set(0);
	addstr( instructions());

	refresh();
	getch();
	clear();

	addstr( gui(str,gas,brk,forward,reverse));
	addstr( txt(str,gas,brk,forward,reverse,parking));
	refresh();

	while(1){

		tmp = getch();
		if(tmp == 104){
			clear();
			addstr( instructions());
			refresh();
			getch();
			clear();
		}else if(tmp == 113){
			clear();
			endwin();
			exit(0);
		}else{
			update(tmp,str,brk,gas,forward,reverse,parking,*spack);
			clear();
			addstr( gui(str,gas,brk,forward,reverse));
			addstr( txt(str,gas,brk,forward,reverse,parking));
			refresh();

			ml.lock();
			//setGas((float)gas, epao);
			//setSteer((float)str, epao);
			//setBrake((float)brk, epao);
			ml.unlock();

		}

	}
	clear();
	endwin();
	return 0;
}


