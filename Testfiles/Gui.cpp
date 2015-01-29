/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */
#include <curses.h>
#include <string>
#include <sstream>
#include <math.h>
#include <unistd.h>
#include <thread>
#include <mutex>

#include "../EBU/EBUManager.h"
#include "../Packets/EBUPacketAnalogOut.h"
#include "../Packets/EBURelayPacket.h"

#define OFF 0
#define ON 1

std::mutex ml;

void ebuSend(Packets::EBUPacketAnalogOut* epao){
	EBU::EBUManager em = EBU::EBUManager();
	Packets::EBURelayPacket rp = Packets::EBURelayPacket();

	rp.setRelayValue(R_A7, ON);
	rp.setRelayValue(R_A17, ON);
	rp.setRelayValue(R_A18, ON);
	rp.setRelayValue(R_A19, ON);
	rp.setRelayValue(R_A20, ON);
	em.sendRelayCommand(rp, 1);

	while(1){
		ml.lock();
		em.sendAnalogCommand(epao->getChannel(),1);
		ml.unlock();
		usleep(200000);
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

void update(int i, double& vdir, double& brks, double& spd, int& gear){
	if(i == 110 && brks>0 && spd < 1){
		gear = 0;
	}else if(i == 114 && brks>0 && spd < 1){
		gear = 2;
	}
	else if(i == 102 && brks>0 && spd < 1){
		gear = 1;
	}else if(i == 32){
		((brks < 4.0) ? brks+=1 :brks);
		spd=0;
	}else if(i == 98){
		((brks >= 0.5) ? brks-=1 :brks);
	}else if(i == 67){
		((vdir < 4.4) ? vdir+=0.1 :vdir);
	}else if(i == 68){
		((vdir > 0.5) ? vdir-=0.1 :vdir);
		//((vdir < 0.5) ? vdir-=0.1 :vdir);
	}else if(i == 66){
		((spd > 0.0 && brks < 1) ? spd-=0.1 :spd);
	}else if(i == 65){
		((spd < 4.5 && brks < 1) ? spd+=0.1 :spd);
	}
}


const char* txt(double vdir, double spd, double brks, int gear){
	std::string v = std::to_string(vdir);
	std::string s = std::to_string(fabs(spd));
	std::string b = std::to_string(brks);
	std::string g;
	if(gear == 1){
		g="F";
	}else if(gear == 2){
		g="R";
	}else{
		g="N";
	}
	std::string tmp = "\n\n"
			"    Stearing at voltage(0-5v):"+v+"\n"
			"    Throttle at voltage(0-5v):"+s+"\n"
			"    Breaks   at voltage(0-5v):"+b+"\n"
			"\n"
			"    Gear:"+g+
			"\n\n\n"
			"    Press \"H\" key for help or \"Q\" to quit\n";

	return tmp.c_str();
}

const char* gui(double vdir, double spd, double brks, int gear){
	std::string tmp;
	std::string v = std::to_string(vdir);
	std::string s = std::to_string(spd);
	std::string b = std::to_string(brks);
	std::string g;

	if(gear == 1){
		g="F";
	}else if(gear ==2){
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


//Fixa vÃ¤rden
void setGas(float value, Packets::EBUPacketAnalogOut* pkt) {
	pkt->setChannelValue((int)(value+0.5), AO_19);
	pkt->setChannelValue((int)(value+0.5), AO_20);
}
void setSteer(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = 5.0 - value ;
	pkt->setChannelValue((int)(value+0.5), AO_17);
	pkt->setChannelValue((int)(temp+0.5), AO_18);
}
void setBrake(float value, Packets::EBUPacketAnalogOut* pkt) {
	pkt->setChannelValue((int)(value+0.5), AO_7);
}
int main()
{

	Packets::EBUPacketAnalogOut* epao =  new Packets::EBUPacketAnalogOut();
	epao->setDestination(1);

	double str = 2.5;
	double brk = 4.0;
	double gas = 0.0;
	int gear   = 0;
	int tmp;


	ml.lock();
	setGas((float)gas, epao);
	setSteer((float)str,epao);
	setBrake((float)brk, epao);
	ml.unlock();

	//ebuSend(epao);
	std::thread worker(ebuSend, epao);
	worker.detach();

	initscr();
	(void)noecho();
	curs_set(0);
	addstr( instructions());
	refresh();
	getch();
	clear();

	addstr( gui(str,gas,brk,gear));
	addstr( txt(str,gas,brk,gear));
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
			update(tmp,str,brk,gas,gear);
			clear();
			addstr( gui(str,gas,brk,gear));
			addstr( txt(str,gas,brk,gear));
			refresh();

			ml.lock();
			setGas((float)gas, epao);
			setSteer((float)str, epao);
			setBrake((float)brk, epao);
			ml.unlock();

		}

	}
	clear();
	endwin();
	return 0;
}


