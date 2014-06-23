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
	sp =  Packets::SimPack();
	epao = Packets::EBUPacketAnalogOut();
	std::thread tOne (&Ground::receivePacket, this);
	std::thread tTwo(&Ground::sendPacket, this);
	tOne.detach();
	tTwo.detach();
}

void Ground::sendPacket() {
	int prio = 1;
	int streamID = 1;
	while(not pleased){
		//usleep(100);
		sp = simulator->recPac();
		if(m_ebuPacket.try_lock() and m_cawlSocket.try_lock()){
			setEbuOne(&sp, &epao);
			char *thetemp;
			thetemp = (char*) malloc(sizeof(epao));
			memcpy(thetemp, &epao, sizeof(epao));
			Packets::CawlPacket *out = new Packets::CawlPacket(prio, streamID);
			memcpy(out->data, thetemp, sizeof(epao));
			try{
				socketOut->send(*out);
			}catch(int e){
				printf("ERROR number %i\n", e);
				perror("Description");
				printf("Packet not sent\n");
				m_ebuPacket.unlock();
				m_cawlSocket.unlock();
				exit(0);
			}
			m_ebuPacket.unlock();
			m_cawlSocket.unlock();
		}
	}


}
void Ground::receivePacket(){
	while(not pleased){
		if(m_ebuPacket.try_lock()){
			Packets::CawlPacket *recPack = new Packets::CawlPacket(0);
			try{
				socketOut->rec(*recPack);
				//printf("PACKET RECEIVED :D\n");
			}catch(int e){
				printf("ERROR %i\n", e);
				perror("Nope");
			}
			char *tempbuff;
			tempbuff = (char*) malloc(sizeof(epao));
			memcpy(tempbuff, recPack->data, sizeof(epao));
			memcpy(&epao, tempbuff, sizeof(epao));
			m_ebuPacket.unlock();
		}
	}
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

//int Ground::PacketHandler() {
//	receivePacket(&epao);
//	sp = simulator->recPac();
//	setEbuOne(&sp, &epao);
//	//printf("Packet to send, data in pin 9 and 10 are: %i, %i\n", epao.getChannelValue(AO_9), epao.getChannelValue(AO_10));
//	sendPacket(1, 1, epao);
//	return 1;
//}

void Ground::setEbuOne(Packets::SimPack* sp, Packets::EBUPacketAnalogOut* epao) {
	epao->setDestination(1);
	Packets::commandPacket simData = sp->getData();
	setBoom((float)simData.analog[2], epao);
	setBucket((float)simData.analog[3], epao);
}

Ground::~Ground() {
	// TODO Auto-generated destructor stub
}
void INT_handler(int dummy){
	exit(EXIT_SUCCESS);
}


int main()
{
	signal(SIGINT, INT_handler);
	Netapi::Host client =Netapi::Host((char*)"127.0.0.1", 5555, (char*)"127.0.0.1", false);
	Netapi::CawlSocket* clientSocket = new Netapi::CawlSocket(client);
	clientSocket->setmetrics(true);
	inet_pton(AF_INET, "127.0.0.1", &(clientSocket->addr.sin_addr)); //Lättare att använda, sköter network byte order åt dig.
	clientSocket->addr.sin_port = htons(5555);
	try{
		Packets::CawlPacket worthless = Packets::CawlPacket(1,1);
		clientSocket->send(worthless);
		sleep(1);
	}
	catch(int e)
	{
		printf("error %i\n", e);
		perror("Description");
		exit(0);
	}
	Ground *g = new Ground(clientSocket);
	bool running = true;
	while(running){

	}
	printf("Finished\n");
	return 0;
}
