/*
Purpose: Header for defines of the queue and for the entire project - these project constants could have also been included in the CAN header but later in the project serious changes may be made to that file
 Author: Josh Osborne
   Date: June 6
Version: 1
*/

#ifndef  _QUEUE_H
#define  _QUEUE_H

#define QUEUE_LENGTH 10

// Node Identifiers
#define CONTROLLER	0x00
#define PID 		0x01
#define CAR 		0x02
#define F1 			0x03
#define F2 			0x04
#define F3 			0x05

// PID Identifiers
#define SET_POINT 0x00
#define REC_POINT 0x01

// Elevator Car Identifiers
#define FLOOR_BUTTON 			0x00
#define DOOR 					0x01
#define FLOOR_INDICATOR 		0x02
#define E_STOP 					0x03
#define BUTTON_INDICATOR_ON		0x04
#define BUTTON_INDICATOR_OFF	0x05

// Floor Levels
#define LEVEL_1 0x01
#define LEVEL_2 0x02
#define LEVEL_3 0x03

// Floor Node Identifiers
#define CALL_SWITCH 	0x00
#define CALL_LED 		0x01
#define FLOOR_INDICATOR 0x02

// Characteristics
#define OFF   0x00
#define ON    0x01
#define UP    0x00
#define DOWN  0x01
#define CLOSE 0x00
#define OPEN  0x01


// structure for each FIFO Queue 
typedef struct Link_List
{
	unsigned int capacity;												//Max number of elements the queue can hold
	unsigned int size;													//Current size of the queue
	int front;															//Index of the first element
	int back;															//Index of the last element
	volatile unsigned int elements[QUEUE_LENGTH][DATA_LENGTH];			//Data of the queue node
}Queue;

//MAKE AN ENUM FOR RETURN ERRORS

int IsQueueEmpty(Queue Q);
int AddToQueue(Queue *Q);
int DeQueue(Queue *Q);
void InitializeQueue(Queue *Q);

extern volatile unsigned char Queue_Flag;

extern Queue rec;

#endif