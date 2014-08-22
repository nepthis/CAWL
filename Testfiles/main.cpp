#include <curses.h>
#include <string>
#include <sstream>
#include <math.h>
#include <unistd.h>

#include "../EBU/EBUManager.h"
#include "../Packets/EBUPacketAnalogOut.h"
#include "../Packets/EBURelayPacket.h"

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
void setGas(float value, Packets::EBUPacketAnalogOut* pkt) {
	pkt->setChannelValue(value, AO_19);
	pkt->setChannelValue(value, AO_20);
}
void setSteer(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = 5.0 - value ;
	pkt->setChannelValue(value, AO_17);
	pkt->setChannelValue(temp, AO_18);
}
void setBrake(float value, Packets::EBUPacketAnalogOut* pkt) {
	pkt->setChannelValue(value, AO_7);
}
int main()
{
	EBU::EBUManager em = EBU::EBUManager();
	Packets::EBUPacketAnalogOut epao =  Packets::EBUPacketAnalogOut();
	Packets::EBURelayPacket rp = Packets::EBURelayPacket();
	int off =0;
	int on = 1;
	rp.setRelayValue(R_A7, on);
	rp.setRelayValue(R_A17, on);
	rp.setRelayValue(R_A18, on);
	rp.setRelayValue(R_A19, on);
	rp.setRelayValue(R_A20, on);
	epao.setDestination(1);
	double str = 2.5;
	double brk = 4.0;
	double gas = 0.0;
	int gear   = 0;
	em.sendRelayCommand(rp, 1);
	setGas((float)gas, &epao);
	setSteer((float)str,& epao);
	setBrake((float)brk, &epao);
	em.sendAnalogCommand(epao.getChannel(), 1);
	int tmp;

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
		usleep(20000);
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
			setGas((float)gas, &epao);
			setSteer((float)str, &epao);
			setBrake((float)brk,& epao);
			em.sendAnalogCommand(epao.getChannel(), 1);
			printf("package sent with values %f, %f, %f\n", gas, str, brk);
			refresh();
		}

	}
	clear();
	endwin();
	return 0;
}


