/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */

#include <stdio.h>
#include <string>
#include <stdint.h>
#include "../Netapi/Host.h"

using namespace Netapi;

int main(){
	Host host = Host((char*)"10.0.0.1",231);
	host.SetBindAddr((char*)"10.0.0.55");
	host.SetServerAddr((char*)"10.0.0.3");
	host.SetServer(true);
	host.SetPort(432);


	std::string boo;

	if(host.GetServer()){
		boo = "True";
	}else{
		boo = "False";
	}

	printf("Address1: %s , Address2: %s , Port: %i ,IsServer: %s \n", host.GetServerAddr() , host.GetBindAddr(),  host.GetPort(), boo.c_str());
	return 0;
}


