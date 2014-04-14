#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
/*
don't forget to include the extended socket

pressing the button for the desired function will increment voltage with 1.
Max level is 5 and pushing "opposite" will result decrease with 1.
IMPORTANT! This is only for keyboard control, will be different when in use
with a joystick or similar
*/


main()
{

	printf("w; raise boom\ts; lower boom\nq; tilt bucket up\te; tilt bucket down\n");
	char c;
	int countBoom = 0;
	int countBuck = 0;
	/*
	socketOutEbuOne = socket();
	socketInEbuOne = socket();
	*/

	//Sockets to/from the simulator will be added together with more functions

	while((c = getchar()) != EOF)
		c = tolower(c);
		/*
		After editing the inputchar from keyboard to lowercase print it
		and use a switch to determine the right function for the desired
		action.
		*/
		switch(c)
			case "w":
				putchar(c);
				if (countBoom <5)
					countBoom++;
					printf("Current level: %d\n", countBoom);
					/*
						just send the character, this keeps the amount of data down 
						on the link, the struct will be created on the otehr side

						outSocket send command as char + voltage as int
					*/
				else
					printf("At max level\n");
				break;

			case "s":
				putchar(c);
				if(countBoom > -5)
					countBoom--;
					printf("Current level: %d\n", countBoom);	
				else
					printf("At minimum level\n");
				break;

			case "q":
				putchar(c);
				if(countBuck > -5)
					countBuck--;
					printf("Current level: %d\n", countBuck);	
				else
					printf("At minimum level\n");

				break;

			case "e":
				putchar(c);
				break;

			default:
				printf("Unrecognized input\n")

	printf("Finished");
	return 0;
}
