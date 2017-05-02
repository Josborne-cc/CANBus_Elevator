/*
Purpose: Header file for CAN Bus access functions
   Date: June 6
 Author: Josh Osborne
Version: 1
*/

#ifndef _CAN_H
#define _CAN_H

#define DATA_LENGTH 3
#define CAN_PRIORITY_ID 0x0000


// buffers for use with the CAN transmission
extern volatile unsigned char rxbuf[DATA_LENGTH];
extern volatile unsigned char txbuf[DATA_LENGTH];


void CANinit(unsigned char id);
interrupt 38 void RxISR(void);
unsigned char CANSend(unsigned char id, unsigned char rtr, unsigned char priority, unsigned char length, unsigned char *txdata);

#endif