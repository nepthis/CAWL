/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */
#include <stdio.h>
#include <unistd.h>
#include <typeinfo>
#include <cstring>
#include <iostream>
#include "../Packets/SimPack.h"

using namespace Packets;

int main(){

	SimPack temp1 = SimPack();
	SimPack temp2 = SimPack();
	SimPack temp3 = SimPack();
	SimPack temp4 = SimPack();
	SimPack temp5 = SimPack();

	temp1.fromSim.digital = 12321;
	temp1.fromSim.analog[0] = 4501;
	temp1.fromSim.analog[1] = 4502;
	temp1.fromSim.analog[2] = 4503;
	temp1.fromSim.analog[3] = 4504;
	temp1.fromSim.analog[4] = 4505;
	temp1.fromSim.analog[5] = 4506;
	temp1.fromSim.analog[6] = 4507;
	temp1.fromSim.analog[7] = 4508;

	temp2.fromSim.digital = 12321;
	temp2.fromSim.analog[0] = 4501;
	temp2.fromSim.analog[1] = 4502;
	temp2.fromSim.analog[2] = 4503;
	temp2.fromSim.analog[3] = 4504;
	temp2.fromSim.analog[4] = 4505;
	temp2.fromSim.analog[5] = 4506;
	temp2.fromSim.analog[6] = 4507;
	temp2.fromSim.analog[7] = 4508;

	temp3.fromSim.digital = 12301;
	temp3.fromSim.analog[0] = 4511;
	temp3.fromSim.analog[1] = 4512;
	temp3.fromSim.analog[2] = 4513;
	temp3.fromSim.analog[3] = 4514;
	temp3.fromSim.analog[4] = 4515;
	temp3.fromSim.analog[5] = 4516;
	temp3.fromSim.analog[6] = 4517;
	temp3.fromSim.analog[7] = 4518;

	temp4.fromSim.digital = 12301;
	temp4.fromSim.analog[0] = 4501;
	temp4.fromSim.analog[1] = 4502;
	temp4.fromSim.analog[2] = 4503;
	temp4.fromSim.analog[3] = 4504;
	temp4.fromSim.analog[4] = 4505;
	temp4.fromSim.analog[5] = 4506;
	temp4.fromSim.analog[6] = 4507;
	temp4.fromSim.analog[7] = 4508;

	temp5.fromSim.digital = 12321;
	temp5.fromSim.analog[0] = 4501;
	temp5.fromSim.analog[1] = 4502;
	temp5.fromSim.analog[2] = 4503;
	temp5.fromSim.analog[3] = 4504;
	temp5.fromSim.analog[4] = 4505;
	temp5.fromSim.analog[5] = 4506;
	temp5.fromSim.analog[6] = 4507;
	temp5.fromSim.analog[7] = 4509;

	printf("Temp2: %s , Temp3: %s , Temp4: %s , Temp5: %s \n",(temp1 == temp2) ? "True" : "False" ,(temp1 == temp3) ? "True" : "False" ,(temp1 == temp4) ? "True" : "False" ,(temp1 == temp5) ? "True" : "False");
	for(int i=0;i<32;++i){
		printf("Index%i: %i \n",i,temp1.getDigital(i));
	}
	printf("\n");
}
