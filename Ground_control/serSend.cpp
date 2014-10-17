#include "serSend.h"

serSend::serSend(){
  cport_nr=16;        /* /dev/ttyUSB0 */
  //bdrate=1000000;
  bdrate=9600;
  strcpy(str, "\xA0");

  if(RS232_OpenComport(cport_nr, bdrate)){
	  perror("Comport error");
	  throw 12;

  }
}

serSend::~serSend(){
}

void serSend::sndPulse(){
	RS232_cputs(cport_nr, str);
}
