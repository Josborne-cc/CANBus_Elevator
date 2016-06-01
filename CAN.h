/*
Author: Josh Osborne

*/

#ifndef _CAN_H
#define _CAN_H

#define DATA_LENGTH 3

// buffers for use with the CAN transmission
volatile unsigned int rxbuf[DATA_LENGTH];
volatile unsigned int txbuf[DATA_LENGTH];


void CANinit(void);
interrupt 38 void RxISR(void);
unsigned char CANSend(unsigned long id, unsigned char priority, unsigned char length, unsigned int *txdata);

#endif