#include <unistd.h>
#include "rs232.h"


class serSend {
public:
	serSend();
	virtual ~serSend();
	void sndPulse();

	int cport_nr;        /* /dev/ttyUSB0 */
  	int bdrate;
  	char str[512];
};