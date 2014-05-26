/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: cawl-server
 */

#include "Ground.h"
using namespace std;

Ground::Ground() {
	countBoomUp = countBuckUp = countBoomDown = countBuckDown = pleased = 0;
	c = new Netapi::Host((char*)"130.240.109.70", 1235, (char*)"127.0.0.1");
	socketOut = new Netapi::CawlSocket(*c);
	simulator = Sim();
}
//used for testing EBU only, does not really handle or send data in correct way.
//void Ground::handleInput() {
//	int c;
//	while((c = getch()) != EOF){
//		c = tolower(c);
//		switch(c){
//		case 'w':
//			putchar(c);
//			countBoomDown = 0;
//			boomDown(countBoomDown);
//			if (countBoomUp < 5)
//			{
//				countBoomUp++;
//				printf("\nBoom level: %d\n", countBoomUp);
//				boomUp(countBoomUp);
//			}
//			else
//			{
//				printf("\nBoom at max level\n");
//				boomUp(countBoomUp);
//			}
//			break;
//
//		case 's':
//			putchar(c);
//			countBoomUp = 0;
//			boomUp(0);
//			if(countBoomDown > -5)
//			{
//				countBoomDown--;
//				printf("\nBoom level: %d\n", countBoomDown);
//				boomDown((countBoomDown*-1));
//			}
//			else
//			{
//				printf("\nBoom at min level\n");
//				boomDown((countBoomDown*-1));
//			}
//			break;
//		case 'q':
//			putchar(c);
//			countBuckDown = 0;
//			bucketDown(countBuckDown);
//			if(countBuckUp < 5)
//			{
//				countBuckUp++;
//				printf("\nBucket level: %d\n", countBuckUp);
//				bucketUp(countBuckUp);
//			}
//			else
//			{
//				printf("\nBucket at max level\n");
//				bucketUp(countBuckUp);
//			}
//			break;
//
//		case 'e':
//			putchar(c);
//			countBuckUp = 0;
//			bucketUp(countBuckUp);
//			if(countBuckDown > -5)
//			{
//				countBuckDown--;
//				printf("\nBucket level: %d\n", countBuckDown);
//				bucketDown(countBuckDown);
//			}
//			else
//			{
//				printf("\nBucket at min level\n");
//				bucketDown((countBuckDown*-1));
//			}
//			break;
//
//
//		default:
//			printf("\nUnrecognized input\n");
//			break;
//		}
//	}
//	pthread_exit(NULL);
//}


////type, 1 = analog
//int Ground::boomUp(uint8_t voltage) {
//	uint8_t type =1;
//	uint8_t pin = AO_9;
//	uint8_t ebuNum = 1;
//	sendPacket(createPacket(type, pin, voltage, ebuNum));
//	return 1;
//}
//
//int Ground::boomDown(uint8_t voltage) {
//	uint8_t type =1;
//	uint8_t pin = AO_10;
//	uint8_t ebuNum = 1;
//	sendPacket(createPacket(type, pin, voltage, ebuNum));
//	return 1;
//}
//
//int Ground::bucketUp(uint8_t voltage) {
//	uint8_t type =1;
//	uint8_t pin = AO_11;
//	uint8_t ebuNum = 1;
//	sendPacket(createPacket(type, pin, voltage, ebuNum));
//	return 1;
//}
//
//int Ground::bucketDown(uint8_t voltage) {
//	uint8_t type =1;
//	uint8_t pin = AO_12;
//	uint8_t ebuNum = 1;
//	sendPacket(createPacket(type, pin, voltage, ebuNum));
//	return 1;
//}

char Ground::getch() {
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

int Ground::sendPacket(int prio, int streamID, Packets::EBUPacketAnalogOut pkt) {
	char temp[sizeof(pkt)];
	memcpy(&temp, &pkt, sizeof(&pkt));
	Packets::CawlPacket out = Packets::CawlPacket(prio, streamID, temp);
	socketOut->send(out);
	return 1;
}

int Ground::setBoom(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value  * 2 + 2.5;
	pkt->setChannelValue(5-temp, AO_9);
	pkt->setChannelValue(temp, AO_10);
	return 1;
}

int Ground::setBucket(float value, Packets::EBUPacketAnalogOut* pkt) {
	float temp = value * 2 + 2.5;
	pkt->setChannelValue(5-temp, AO_11);
	pkt->setChannelValue(temp, AO_12);
	return 1;
}

int Ground::PacketHandler() {
	Packets::SimPack sp = Packets::SimPack();
	Packets::EBUPacketAnalogOut epao = Packets::EBUPacketAnalogOut();
	int toggle = 0;
	while(not pleased){
		if (toggle){
			epao.setChannelValue(0,AO_9);
			epao.setChannelValue(5,AO_10);
		}else{
			epao.setChannelValue(5,AO_9);
			epao.setChannelValue(1,AO_10);
		}
		//sp = simulator.recPac();
		//setEbuOne(sp, epao);
		sendPacket(1, 1, epao);
	}
	return 1;
}

int Ground::setEbuOne(Packets::SimPack sp, Packets::EBUPacketAnalogOut* epao) {
	epao->setDestination(1);
	commandPacket simData = sp.getData();
	setBoom((float)simData.analog[2], epao);
	setBucket((float)simData.analog[3], epao);
	return 1;
}

Ground::~Ground() {
	// TODO Auto-generated destructor stub
}
void INT_handler(int dummy){
	exit(EXIT_SUCCESS);
}

void runThread(Ground *g) {
	g->PacketHandler();
}

int main()
{
	signal(SIGINT, INT_handler);
	Ground g = Ground();
	g.PacketHandler();
	//	pthread_t t1;
	//	pthread_create(&t1, NULL, runThread, (void*)g);
	//	printf("Threads created, awaiting input\n");
	//	printf("w = raise boom\ts = lower boom\nq = tilt bucket up\te = tilt bucket down\n");
	//	pthread_join(t1,NULL);
	printf("Finished");
	return 0;
}
