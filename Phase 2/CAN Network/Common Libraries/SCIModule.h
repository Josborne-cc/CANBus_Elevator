/*
Purpose: Serial Communication Macros
Author:  Josh Osborne
Date:    April 6, 2015
Version: 00
*/

#ifndef _SCIModule_H
#define _SCIModule_H

// Macro to define length of serial buffer
#define SCI_Rx_BUF_LENGTH 16

// Macro to define baud rate of 9600
#define BAUD_RATE_DIVISOR_9600 52

// Macro to define the use of N81 data
#define N81_DATA 0

#define SERIAL_QUEUE_LENGTH 10
#define SERIAL_QUEUE_DATA   3

extern volatile char SCI_Rx_BUF[SCI_Rx_BUF_LENGTH];
extern volatile unsigned int SCI_Rx_INDEX;
extern int CMD_FLAG;
extern int COMM_ERROR;
extern int CMD_START;
extern unsigned long MESSAGE_COUNT;
extern volatile unsigned char Serial_CMD[SERIAL_QUEUE_DATA];
extern volatile unsigned char Serial_Recieve[SERIAL_QUEUE_DATA];

extern int Serial_Count;

typedef struct Serial_Link_List
{
    unsigned int capacity;
    unsigned int size;
    int front;
    int rear;
    unsigned char elements[SERIAL_QUEUE_LENGTH][SERIAL_QUEUE_DATA];
}Serial_Queue;

extern Serial_Queue Serial_In;

//prototypes
void putcSCI(char cx);
void putsSCI(char *str);
void getcSCI(char cx);
void SCIgetc(char buf);
void getsSCI(int num, char *str);
void SCIprintf(char *format, ...); 
interrupt VectorNumber_Vsci void SCI_HANDLER( void );
void SCIinit(int BAUD_RATE, int SCI_DATA, int CONFIG);

void Serial_Create_Queue(Serial_Queue *Q);
int Serial_Dequeue(Serial_Queue *Q);
int  Serial_AddToQueue(Serial_Queue *Q);
int  Serial_IsQueueEmpty(Serial_Queue Q);

#endif