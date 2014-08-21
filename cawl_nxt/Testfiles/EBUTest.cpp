#include"../EBU/EBUManager.h"				//For communication to EBU
#include "../Packets/EBUPacketAnalogOut.h"	//Class/struct for information to the EBU, also contains defines for indexing
#include "../Packets/EBURelayPacket.h"	//In order to set which relays easier, it contains all defines.

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <termios.h>

typedef struct state{
	float bucket;
	float boom;
	float gas;
	float brake;
	float steering;
	int gearFor;
	int gearBack;
}state;

char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}
int main(){
	state s = {0.0, 0.0, 0.0, 0.0, 0.0, 0, 0};
	EBU::EBUManager em = EBU::EBUManager();
	Packets::EBUPacketAnalogOut 	eao 	= Packets::EBUPacketAnalogOut();
	Packets::EBURelayPacket				er		= Packets::EBURelayPacket();
	eao.setDestination(1);

	er.setRelayValue(R_A9,1);
	er.setRelayValue(R_A10,1);
	er.setRelayValue(R_A11,1);
	er.setRelayValue(R_A12,1);
	er.setRelayValue( R_A7, 1);
	er.setRelayValue( R_A17, 1);
	er.setRelayValue( R_A18, 1);
	em.sendRelayCommand(er, 1);

	while (1){
		usleep(20000);
		i =i  + 0.1;
		if(i > 1.0){
			i = -1.0;
		}
		float temp = (i * 2 + 2.5)+0.5;

		//eao.setChannelValue(i, AO_9);
		//eao.setChannelValue(i, AO_10);
		//eao.setChannelValue(i, AO_11);
		//eao.setChannelValue(i, AO_12);
		//eao.setChannelValue(i, AO_7)
		eao.setChannelValue(5-temp, AO_17);
		eao.setChannelValue(temp, AO_18);
		em.sendAnalogCommand(eao,eao.getDestination());
	}
	er.setRelayValue(R_A9,0);
	er.setRelayValue(R_A10,0);
	er.setRelayValue(R_A11,0);
	er.setRelayValue(R_A12,0);
	er.setRelayValue( R_A17, 0);
	em.sendRelayCommand(er, 1);
	return 0;
}
