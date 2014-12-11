/*
 *  Authors: Robin Bond & Håkan Therén
 *  Feel free to copy, use, and modify the code as you see fit.
 *  If you have any questions, look in the bitbucket wiki.
 *  https://bitbucket.org/bondue/cawl_nxt/wiki/Home
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>

int main(){
	/*
	 * The Test will run on the same host for now
	 *
	 */
	struct sockaddr_in appAddr;
	int servSock;

	if ((servSock = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		perror("socket error");
		return 0;
	}
	memset((char *)&appAddr, 0, sizeof(appAddr));
	appAddr.sin_family = AF_INET;
	appAddr.sin_addr.s_addr = htonl('0.0.0.0');
	appAddr.sin_port = htons(0);
	if (bind(servSock, (struct sockaddr *)&appAddr, sizeof(appAddr)) < 0) {
		perror("error with bind");
		return 0;
	}

	return 0;
}



 
