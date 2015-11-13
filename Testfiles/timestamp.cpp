#include <time.h>
#include <chrono>
#include <vector>
#include <stdio.h> //Used for printf
#include <unistd.h>
#include <iostream>
#include <typeinfo>

#include "../Packets/SimPack.h"

using namespace std::chrono;

int main(void){
	std::vector<Packets::SimPack> packets;
	//std::chrono::system_clock::time_point start = high_resolution_clock::now();
	for (int i =0; i< 123;i++){
		//std::chrono::system_clock::time_point now = high_resolution_clock::now();
		Packets::SimPack current = Packets::SimPack();
		current.stampTime();
		packets.push_back(current);
		usleep(1337);
	}
		auto then = packets.front().fromSim.timeStamp;
		auto now = packets.back().fromSim.timeStamp;
		std::cout << "Duration since start: " << std::chrono::duration_cast<std::chrono::milliseconds>(now - then).count()<< " milliseconds\n";

	//Packets::SimPack sp = Packets::SimPack();

}
