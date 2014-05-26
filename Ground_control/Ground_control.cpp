/*
 * Ground_control.cpp
 * Author: Robin Bond
 * Description:
 * 	Main program for creating objects for simulator and communication for the
 * 	gateway at the control site.
 *
 */
#include "Ground_control.h"

Packets::CawlPacket pktOut = Packets::CawlPacket((uint8_t)1);



//Packets::EBUPacketAnalogOut  analogPacket = Packets::EBUPacketAnalogOut();

Netapi::Host client = Netapi::Host((char*)"127.0.0.1", 1235, (char*)"127.0.0.1");
Netapi::CawlSocket socketOut = Netapi::CawlSocket(client);

//EBU::EBUManager em = EBU::EBUManager();

queue<Packets::CawlPacket> toGateWay;

pthread_mutex_t qCawl = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t packetOut = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t packet = PTHREAD_MUTEX_INITIALIZER;

//pthread_mutex_t m_ebuman = PTHREAD_MUTEX_INITIALIZER;

/*
 * enQueuePacket takes the data and a char buffer and copies in the data into the buffer
 * afterwards it creates a packet and copies it into a queue. (I think it copies the packet, at least)
 */
void *deQueue(void *parg){
	while(not pleased){
//		pthread_mutex_lock(&threadQueue);
//		while (!running_queue)
//			pthread_cond_wait(&t_queue, &threadQueue);
//		running_queue = 0;
//		pthread_mutex_unlock(&threadQueue);
		//pthread_mutex_lock(&qCawl);
		//pthread_mutex_lock(&packetOut);
		if(toGateWay.empty()){
			printf("Queue is empty\n");
			//pthread_mutex_unlock(&qCawl);
			//pthread_mutex_unlock(&packetOut);
		}else{
			pktOut = toGateWay.front();
			toGateWay.pop();
			socketOut.send(pktOut);
			//pthread_mutex_unlock(&qCawl);
			//pthread_mutex_unlock(&packetOut);
		}
//	    pthread_mutex_lock(&threadHandle);
//	    running_handle= 1;
//	    pthread_cond_signal(&t_handle);
//	    pthread_mutex_unlock(&threadHandle);
//		//em.sendAnalogCommand()

	}
	pthread_exit(NULL);
}
void enQueuePacket(uint8_t t, int  p, int v,int n){
	pthread_mutex_lock(&packetOut);
	pthread_mutex_lock(&qCawl);
	printf("before copy\n");
	printf("to be copied: %c, %i, %i, %i\n", t,p,v,n);
//	memcpy(g_buffer+0, &t, sizeof(uint8_t));
//	memcpy(g_buffer+8, &p, sizeof(int));
//	memcpy(g_buffer+40, &v, sizeof(int));
//	memcpy(g_buffer+72, &n, sizeof(int));
	dc.type = t;
	dc.pin = p;
	dc.value = v;
	dc.ebuNum = n;
	printf("after copy\n");
	printf("\nPacket contains: %c, %i, %i, %i\n", dc.type, dc.pin, dc.value,dc.ebuNum);
	pktOut.data=(char*)&dc;
	printf("packet buffer contains: %c, %i, %i, %i\n", (uint8_t)pktOut.data[0],(int)pktOut.data[8],(int)pktOut.data[40], (int)pktOut.data[72]);
	pktOut.prio = 1;
	pktOut.streamId = 1;
	socketOut.send(pktOut);
	//toGateWay.push(pktOut);
	printf("Packet PUSHED\n");
	pthread_mutex_unlock(&qCawl);
	pthread_mutex_unlock(&packetOut);
	//printf("locks unlocked\n");
}
void boomUp(int voltage){
	uint8_t type =1;
	int pin = AO_9;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum);
	//enQueuePacket(type, pin, value, ebuNum,buffer);
	//pthread_mutex_lock(&packet);
	//pthread_mutex_lock(&m_ebuman);
	//printf("\nvoltage to be written: %d\n", voltage);
	//analogPacket.setChannelValue(voltage, AO_9);
	//sleep(0);
	//uint16_t temp = analogPacket.getChannelValue(AO_9);
	//printf("\nthe value in the array is: %u\n", temp);
	//em.sendAnalogCommand(analogPacket,ebuNum);
	//pthread_mutex_unlock(&m_ebuman);
	//pthread_mutex_unlock(&packet);


}
void boomDown(int voltage){
	uint8_t type =1;
	int pin = AO_10;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum);
	//enQueuePacket(type, pin, value, ebuNum, buffer);
	//	pthread_mutex_lock(&packet);
	//	pthread_mutex_lock(&m_ebuman);
	//	printf("\nvoltage to be written: %d\n", voltage);
	//	analogPacket.setChannelValue(voltage, AO_10);
	//	sleep(1);
	//	uint16_t temp = analogPacket.getChannelValue(AO_10);
	//	printf("\nthe value in the array is: %u\n", temp);
	//	em.sendAnalogCommand(analogPacket,ebuNum);
	//	pthread_mutex_unlock(&m_ebuman);
	//	pthread_mutex_unlock(&packet);
}
void bucketUp(int voltage){
	uint8_t type =1;
	int pin = AO_11;
	int ebuNum = 1;
	enQueuePacket(type, pin, voltage, ebuNum);
	//enQueuePacket(type, pin, value, ebuNum, buffer);
	//	pthread_mutex_lock(&packet);
	//	pthread_mutex_lock(&m_ebuman);
	//	analogPacket.setChannelValue(voltage, AO_11);
	//	em.sendAnalogCommand(analogPacket,ebuNum);
	//	pthread_mutex_unlock(&m_ebuman);
	//	pthread_mutex_unlock(&packet);
}
void bucketDown(int voltage){
	uint8_t type =1;
	int pin = AO_12;
	int ebuNum = 1;

	enQueuePacket(type, pin, voltage, ebuNum);

	//enQueuePacket(type, pin, value, ebuNum, buffer);
	//	pthread_mutex_lock(&packet);
	//	pthread_mutex_lock(&m_ebuman);
	//	analogPacket.setChannelValue(voltage, AO_12);
	//	em.sendAnalogCommand(analogPacket,ebuNum);
	//	pthread_mutex_unlock(&m_ebuman);
	//	pthread_mutex_unlock(&packet);
}

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
/*
pressing the button for the desired function will increment voltage with 1.
Max level is 5 and pushing "opposite" will result decrease with 1.
IMPORTANT! This is only for keyboard control, will be different when in use
with a joystick or similar from the simulator.
 */
void *handleInput(void *parg){
	int c;
	while((c = getch()) != EOF){
//		pthread_mutex_lock(&threadHandle);
//		while (!running_handle)
//			pthread_cond_wait(&t_handle, &threadHandle);
//		running_handle= 0;
//		pthread_mutex_unlock(&threadHandle);
		c = tolower(c);
		switch(c){
		case 'w':
			putchar(c);
			countBoomDown = 0;
			boomDown(countBoomDown);
			if (countBoomUp < 5)
			{
				countBoomUp++;
				printf("\nBoom level: %d\n", countBoomUp);
				boomUp(countBoomUp);
			}
			else
			{
				printf("\nBoom at max level\n");
				boomUp(countBoomUp);
			}
			break;

		case 's':
			putchar(c);
			countBoomUp = 0;
			boomUp(0);
			if(countBoomDown > -5)
			{
				countBoomDown--;
				printf("\nBoom level: %d\n", countBoomDown);
				boomDown((countBoomDown*-1));
			}
			else
			{
				printf("\nBoom at min level\n");
				boomDown((countBoomDown*-1));
			}
			break;
		case 'q':
			putchar(c);
			countBuckDown = 0;
			bucketDown(countBuckDown);
			if(countBuckUp < 5)
			{
				countBuckUp++;
				printf("\nBucket level: %d\n", countBuckUp);
				bucketUp(countBuckUp);
			}
			else
			{
				printf("\nBucket at max level\n");
				bucketUp(countBuckUp);
			}
			break;

		case 'e':
			putchar(c);
			countBuckUp = 0;
			bucketUp(countBuckUp);
			if(countBuckDown > -5)
			{
				countBuckDown--;
				printf("\nBucket level: %d\n", countBuckDown);
				bucketDown(countBuckDown);
			}
			else
			{
				printf("\nBucket at min level\n");
				bucketDown((countBuckDown*-1));
			}
			break;


		default:
			printf("\nUnrecognized input\n");
			break;
		}
//		pthread_mutex_lock(&threadQueue);
//		running_queue = 1;
//		pthread_cond_signal(&t_queue);
//		pthread_mutex_unlock(&threadQueue);
	}
	pthread_exit(NULL);
}
/*
 * Function for sending the data through the CAWL socket to the other Gateway
 * Locks the FIFO queue and the packet required to hold the data temporarily
 * and then sends it.
 */

void INT_handler(int dummy){
	pleased = 1;
	exit(EXIT_SUCCESS);
}
int main()
{

	signal(SIGINT, INT_handler);

	//	Packets::EBURelayPacket rp = Packets::EBURelayPacket();
	//	rp.setRelayValue(R_A9, 0);
	//	rp.setRelayValue(R_A10, 0);
	//	rp.setRelayValue(R_A11, 0);
	//	rp.setRelayValue(R_A12, 0);
	//	printf("values of the array are %d, %d, %d, %d\n",rp.getRelayValue(R_A9),rp.getRelayValue(R_A10),rp.getRelayValue(R_A11),rp.getRelayValue(R_A12));
	//	rp.setRelayValue(R_A9, 1);
	//	rp.setRelayValue(R_A10, 1);
	//	rp.setRelayValue(R_A11, 1);
	//	rp.setRelayValue(R_A12, 1);
	//	printf("The values set are %d, %d, %d and %d\n", R_A9, R_A10, R_A11, R_A12);
	//	printf("values of the array are %d, %d, %d, %d\n",rp.getRelayValue(R_A9),rp.getRelayValue(R_A10),rp.getRelayValue(R_A11),rp.getRelayValue(R_A12));
	//
	//	sleep(1);
	//	printf("relay packet is configured, will send in 2s\n");
	//	sleep(2);
	//	em.sendRelayCommand(rp,1);
	//	printf("Packet sent. Creating worker-thread\n");

	pthread_t t1;
	//pthread_t t2;
	pthread_create(&t1, NULL, handleInput, NULL);
	//pthread_create(&t2, NULL, deQueue, NULL);

	//	if (err1 or err2){
	//		perror("Something went wrong with the thread");
	//		printf("\nError number%i", errno);
	//	}
	printf("Threads created, awaiting input\n");
	printf("w = raise boom\ts = lower boom\nq = tilt bucket up\te = tilt bucket down\n");
	pthread_join(t1,NULL);
	//pthread_join(t2,NULL);

	printf("Finished");
	return 0;
}

