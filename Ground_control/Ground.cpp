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
int Ground::receivePacket(Packets::EBUPacketAnalogOut* incoming){
	Packets::CawlPacket recPack =  Packets::CawlPacket(0);
	try{
		socketOut->rec(recPack);
		//printf("PACKET RECEIVED :D\n");
	}catch(int e){
		printf("ERROR %i\n", e);
		perror("Nope");
	}
	char *tempbuff;
	tempbuff = (char*) malloc(sizeof(incoming));
	memcpy(tempbuff, recPack.data, sizeof(incoming));
	memcpy(incoming, tempbuff, sizeof(incoming));
	return 0;
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
	receivePacket(&epao);
	printf("Packet reveiced, data in pin 9 and 10 are: %i, %i\n", epao.getChannelValue(AO_9), epao.getChannelValue(AO_10));
	sp = simulator->recPac();
	setEbuOne(&sp, &epao);
	printf("Packet to send, data in pin 9 and 10 are: %i, %i\n", epao.getChannelValue(AO_9), epao.getChannelValue(AO_10));
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
