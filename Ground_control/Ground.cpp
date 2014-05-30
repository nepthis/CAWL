/*
 * Ground.cpp
 *
 *  Created on: May 19, 2014
 *      Author: cawl-server
 */

#include "Ground.h"
using namespace std;

Ground::Ground(Netapi::CawlSocket* gw_socket) {
	countBoomUp = countBuckUp = countBoomDown = countBuckDown = 0;
	pleased = 0;
	socketOut =  gw_socket;
	simulator = new Simulator::Sim();
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
	char *thetemp;
	//printf("Values in the analog packet to be sent;\n%i, %i, %i, %i", pkt.getChannelValue(AO_9),pkt.getChannelValue(AO_10),pkt.getChannelValue(AO_11),pkt.getChannelValue(AO_12));
	thetemp = (char*) malloc(sizeof(pkt));
	memcpy(thetemp, &pkt, sizeof(pkt));
	Packets::CawlPacket out = Packets::CawlPacket(prio, streamID);
	memcpy(out.data, thetemp, sizeof(pkt));
	try{
		//printf("sending\n");
		socketOut->send(out);
		//printf("sent\n");
	}catch(int e){
		printf("ERROR number %i\n", e);
		perror("Description");
		printf("Packet not sent\n");
		exit(-1);
	}

	//free( thetemp);
	//delete &out;
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
	Packets::SimPack sp =  Packets::SimPack();
	Packets::EBUPacketAnalogOut epao =  Packets::EBUPacketAnalogOut();
	sp = simulator->recPac();
	//printf("Packet received from simulator with ID %i\n", sp.fromSim.packetId);
	setEbuOne(&sp, &epao);
	//printf("epao contains %i %i\n", epao.getChannelValue(AO_9),epao.getChannelValue(AO_10));
	sendPacket(1, 1, epao);

	return 1;
}

int Ground::setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao) {
	epao->setDestination(1);
	Packets::commandPacket simData = sp->getData();
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

//void runThread(Ground *g) {
//	g->PacketHandler();
//}

int main()
{
	signal(SIGINT, INT_handler);
	Netapi::Host client =Netapi::Host((char*)"127.0.0.1", 5555, (char*)"127.0.0.1", false);

	try{
		Netapi::CawlSocket* clientSocket = new Netapi::CawlSocket(client);
		inet_pton(AF_INET, "127.0.0.1", &(clientSocket->addr.sin_addr)); //Lättare att använda, sköter network byte order åt dig.
		clientSocket->addr.sin_port = htons(5555);
		Packets::CawlPacket worthless = Packets::CawlPacket(1,1);
		clientSocket->send(worthless);
		Ground g = Ground(clientSocket);
		while (1){
			g.PacketHandler();
		}
	}
	catch(int e)
	{
		printf("error %i\n", e);
		perror("Description");
		exit(0);
	}



	//	Packets::SimPack sp = Packets::SimPack();
	//	Packets::EBUPacketAnalogOut epao = Packets::EBUPacketAnalogOut();
	//	sp = g.simulator->recPac();
	//	g.setEbuOne(&sp, &epao);
	//	char *thetemp;
	//	thetemp = (char*) malloc(sizeof(epao));
	//	memcpy(thetemp, &epao, sizeof(epao));
	//	Packets::CawlPacket out = Packets::CawlPacket(1, 1);
	//	memcpy(out.data, thetemp, sizeof(epao));
	//	try{
	//		g.socketOut.send(out);
	//	}catch(int e){
	//		printf("ernor thrown : %i\n", errno);
	//		perror("Not Good");
	//		exit(0);
	//	}
	//	sleep(5);

	//	pthread_t t1;
	//	pthread_create(&t1, NULL, runThread, (void*)g);
	//	printf("Threads created, awaiting input\n");
	//	printf("w = raise boom\ts = lower boom\nq = tilt bucket up\te = tilt bucket down\n");
	//	pthread_join(t1,NULL);
	printf("Finished");
	return 0;
}
