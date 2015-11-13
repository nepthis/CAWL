/*
 * EBURelayPacket.h
 *
 *  Created on: May 5, 2014
 *      Author: Robin Bond
 *      Desc: class for the struct that contains the array for the relays + defines for using shorthand names
 *      for the different relays. Most of this is borrowed from the Wheel Loader Project group.
 *
 */

#ifndef RELAYOUT_H_
#define RELAYOUT_H_
/* Pin X is for the ECU pinout (i.e pin 1 represents VA1 on EBU 1 or V2A1 on EBU 2).
 * The defines are Relay on the EBU followed by the index in the bytearray we have to store the values.
 * On the physical EBU the numbers can be read on the ECU-side contact
 */
//---------------------------Analog------------------------
#define R_A1 	105	//Pin 1	Contact A OBS! Only EBU 2, V2 ECU
#define R_A2 	107	//Pin 2 Contact A OBS! Only EBU 2, V2 ECU
#define R_A3 	96		//Pin 3 Contact A OBS! Only EBU 2, V2 ECU
#define R_A4 	99		//Pin 4 Contact A OBS! Only EBU 2, V2 ECU
#define R_A5 	102	//Pin 5 Contact A OBS! Only EBU 2, V2 ECU
#define R_A6 	90		//Pin 6 Contact A OBS! Only EBU 2, V2 ECU
#define R_A7 	93		//Pin 7 Contact A OBS! Only EBU 2, V2 ECU

#define R_A8 	108	//Pin 26 Contact A
#define R_A9 	97		//Pin 27 Contact A
#define R_A10 	100	//Pin 28 Contact A
#define R_A11 	88		//Pin 29 Contact A
#define R_A12 	91		//Pin 30 Contact A
#define R_A13 	94		//Pin 31 Contact A

#define R_A14 	104	//Pin 48 Contact A
#define R_A15 	106	//Pin 49 Contact A
#define R_A16 	109	//Pin 50 Contact A
#define R_A17 	98		//Pin 51 Contact A
#define R_A18 	101	//Pin 52 Contact A
#define R_A19 	89		//Pin 53 Contact A
#define R_A20 	92		//Pin 54 Contact A
//---------------------------Digital------------------------
#define R_D1 	80	//Pin 9 Contact A
#define R_D2 	82	//Pin 10 Contact A
#define R_D3 	85	//Pin 11 Contact A
#define R_D4 	72	//Pin 12 Contact A
#define R_D5 	74	//Pin 13 Contact A
#define R_D6 	77	//Pin 14 Contact A
#define R_D7 	64	//Pin 15 Contact A
#define R_D8 	67	//Pin 16 Contact A

#define R_D9 	83	//Pin 34 Contact A
#define R_D10 	86	//Pin 35 Contact A
#define R_D11 	73	//Pin 36 Contact A
#define R_D12 	76	//Pin 37 Contact A
#define R_D13 	79	//Pin 38 Contact A
#define R_D14 	66	//Pin 39 Contact A
#define R_D15 	69	//Pin 40 Contact A
#define R_D16 	71	//Pin 41 Contact A
#define R_D17 	57	//Pin 42 Contact A
#define R_D18 	59	//Pin 43 Contact A
#define R_D19 	61	//Pin 44 Contact A
#define R_D20 	63	//Pin 45 Contact A
#define R_D21 	48	//Pin 46 Contact A

#define R_D22 	81	//Pin 57 Contact A
#define R_D23 	84	//Pin 58 Contact A
#define R_D24 	87	//Pin 59 Contact A
#define R_D25 	75	//Pin 60 Contact A
#define R_D26 	78	//Pin 61 Contact A
#define R_D27 	65	//Pin 62 Contact A
#define R_D28 	68	//Pin 63 Contact A
#define R_D29 	70	//Pin 64 Contact A
#define R_D30 	56	//Pin 65 Contact A
#define R_D31 	58	//Pin 66 Contact A
#define R_D32 	60	//Pin 67 Contact A
#define R_D33 	62	//Pin 68 Contact A

#define R_D34 	52	//Pin 1 Contact B
#define R_D35 	53	//Pin 2 Contact B
#define R_D36 	55	//Pin 3 Contact B
#define R_D37 	41	//Pin 4 Contact B
#define R_D38 	43	//Pin 5 Contact B
#define R_D39 	45	//Pin 6 Contact B
//-----------------------Solenoids--------------------
#define R_S1 	32		//Pin 50 Contact B
#define R_S2 	33		//Pin 51 Contact B
#define R_S3 	34		//Pin 52 Contact B
#define R_S4 	35		//Pin 53 Contact B
#define R_S5 	36		//Pin 54 Contact B
#define R_S6 	37		//Pin 55 Contact B
#define R_S7 	24		//Pin 56 Contact B
#define R_S8 	25		//Pin 57 Contact B
#define R_S9 	28		//Pin 58 Contact B
#define R_S10 	29	//Pin 59 Contact B
#define R_S11 	16	//Pin 60 Contact B
#define R_S12 	17	//Pin 61 Contact B
#define R_S13 	20	//Pin 62 Contact B
#define R_S14 	21 	//Pin 63 Contact B
#define R_S15	8		//Pin 64 Contact B
#define R_S16 	9		//Pin 65 Contact B
#define R_S17 	12	//Pin 66 Contact B
#define R_S18 	13	//Pin 67 Contact B
#define R_S19 	0		//Pin 68 Contact B
#define R_S20 	1		//Pin 69 Contact B
#define R_S21 	4		//Pin 70 Contact B
//---------------------------PWM------------------------
#define R_P1 	38		//Pin 8 Contact B
#define R_P2 	39		//Pin 9 Contact B
#define R_P3 	26		//Pin 10 Contact B
#define R_P4 	27		//Pin 11 Contact B
#define R_P5 	30		//Pin 12 Contact B
#define R_P6 	31		//Pin 13 Contact B
#define R_P7 	18		//Pin 14 Contact B
#define R_P8 	19		//Pin 15 Contact B
#define R_P9 	22		//Pin 16 Contact B
#define R_P10 	23	//Pin 17 Contact B
#define R_P11 	10	//Pin 18 Contact B
#define R_P12 	11	//Pin 19 Contact B
#define R_P13 	14	//Pin 20 Contact B
#define R_P14 	15	//Pin 21 Contact B
#define R_P15 	2		//Pin 22 Contact B
#define R_P16 	3		//Pin 23 Contact B
//-------------------------Fuses-----------------------
#define R_F1 	54	//Pin 26 Contact B
#define R_F2 	40	//Pin 27 Contact B
#define R_F3 	42	//Pin 28 Contact B
#define R_F4 	44	//Pin 29 Contact B
#define R_F5 	46	//Pin 30 Contact B
#define R_F6 	47	//Pin 31 Contact B
//-------------------------Other comm-----------------------------
#define R_CAN1 	49 	//Pin 19 L - 20 H Contact A
#define R_CAN2 	51	//Pin 21 L - 22 H Contact A
#define R_J1708 50	//Pin 17 A - 18 B Contact A
//----------------------------Not used---------------------------------
#define R_res1	5			//Pin 1 OBS! Only EBU 1, V ECU
#define R_res2	6			//Pin 2 OBS! Only EBU 1, V ECU
#define R_res3	7 			//Pin 3 OBS! Only EBU 1, V ECU
#define R_res4 	95		//Pin 4 OBS! Only EBU 1, V ECU
#define R_res5 	103	//Pin 5 OBS! Only EBU 1, V ECU
#define R_res6 	110	//Pin 6 OBS! Only EBU 1, V ECU
#define R_res7 	111	//Pin 7 OBS! Only EBU 1, V ECU

#include <stdint.h>

namespace Packets{
//The struct that contains the data for the relays to the EBU
typedef struct EBUrelays{
	uint8_t channel[14];
}EBUrelays ;

/*	EBURelayPackets is a container class with methods that abstract away the
 * 	array containing the data to the EBU.
 */
class RelayOut {
public:
	int destination;
	EBUrelays er;
	RelayOut();
	virtual ~RelayOut();
	EBUrelays getRelays(void);
	void setRelayValue(int relay, int value);
	uint8_t getRelayValue( int relay);
};

}


#endif /* EBURELAYPACKET_H_ */
