#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <termios.h>
using namespace std;
/*
#include <../Net/ExtendedSocket.h>
*/
/*
pressing the button for the desired function will increment voltage with 1.
Max level is 5 and pushing "opposite" will result decrease with 1.
IMPORTANT! This is only for keyboard control, will be different when in use
with a joystick or similar from the simulator.
*/

//void forwardPacket(packet pkt){
//
//
//}


/*
 * Borrowed code for making key presses register without hitting enter.
 * Borrowed from http://stackoverflow.com/a/912796, is used instead of
 * This function is used instead of getchar() from stdio.h.
 */
char getch() {
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

//packet createPacket(string pin, int value){
//	packet output = dataPacket();
//}

int main()
{

	printf("w = raise boom\ts = lower boom\nq = tilt bucket up\te = tilt bucket down\n");
	printf("z = reset boom level\tx = reset bucket level\n");
	int c;
	int countBoom = 0;
	int countBuck = 0;
	/*
	 * TODO: Fixa två till variabler och ändra i switch mellan 0-5 för vardera
	 * det kommer inte att fungera annars då det är 4st funktioner och inte två
	 *
	 *
	 */
	/*
	socketOutEbuOne = socket();
	socketInEbuOne = socket();
	*/
	/* For now, ignore the simulator and take keystrokes as input for the steering.
	 *
	 *
	 *
	 */
	while((c = getch()) != EOF){
		c = tolower(c);
		switch(c){
			case 'w':
				putchar(c);
				if (countBoom < 5)
				{
					countBoom++;
					printf("\nBoom level: %d\n", countBoom);
					//pkt = createPacket()
					//extendedSocket.sendPacket()
				}
				else
				{
					printf("\nBoom at max level\n");
				}
				break;

			case 's':
				putchar(c);
				if(countBoom > -5)
				{
					countBoom--;
					printf("\nBoom level: %d\n", countBoom);
					//pkt = createPacket()
					//extendedSocket.sendPacket()
				}
				else
				{
					printf("\nBoom at min level\n");
				}
				break;
			case 'q':
				putchar(c);
				if(countBuck < 5)
				{
					countBuck++;
					printf("\nBucket level: %d\n", countBuck);
				}
				else
				{
					printf("\nBucket at max level\n");
				}
				break;

			case 'e':
				putchar(c);
				if(countBuck > -5)
				{
					countBuck--;
					printf("\nBucket level: %d\n", countBuck);
				}
				else
				{
					printf("\nBucket at min level\n");
				}
				break;
			case 'z':
				putchar(c);
				countBoom = 0;
				printf("\nBoom level reset\n");
				break;
			case 'x':
				putchar(c);
				countBuck = 0;
				printf("\nBucket level reset\n");
				break;

			default:
				printf("\nUnrecognized input\n");
				break;
		}
	}
	printf("Finished");
	return 0;
}

