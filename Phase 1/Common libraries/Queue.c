/*
Purpose: Queue Functions for storing data
 Author: Josh Osborne
   Date: June 6
Version: 1
*/

#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include "TimerModule.h"
#include "utils.h"

volatile unsigned char Queue_Flag;

Queue rec;


// Returns 0 if queue is empty
int  IsQueueEmpty(Queue Q)
{ 
	int length;

	length = Q.size;

	return length;
}

/*
Purpose: Add node to the head of the queue
  Input: The address of the queue
*/
int  AddToQueue(Queue *Q)
{
	int i;
	// Make sure queue is not full 
	if(Q->size == Q->capacity)
	{
		return -1;
	}
	else
	{
		Q->size++;
		Q->back = Q->back + 1;

		// Fill as a ring buffer
		if(Q->back == Q->capacity)
		{
			Q->back = 0;
		}

		// Insert element at the back
		for(i = 0; i < DATA_LENGTH; i++) 
		{
			Q->elements[Q->back][i] = rxbuf[i];
		}

	}
	return 0;
}

/*
Purpose: Delete a node from the head of the queue and put the data into the usage buffer
  Input: The address of the queue
*/
int DeQueue(Queue *Q)
{
  int i;
	// If Queue size is zero then it is empty. So we cannot pop 
	if(Q->size==0)
	{
		//LCDprintf("Queue is Empty\n");
		return -1;
	}
	else
	{
		//Store the data to use
		for(i = 0; i < DATA_LENGTH; i++)
		  rxdata[i] = Q->elements[Q->front][i];
		
		Q->size--;
		Q->front++;
		// Make it a ring buffer
		if(Q->front == Q->capacity)
		{
			Q->front=0;
		}
		
	}
	
	return 0;
}

/*
Purpose: Initialize a Queue
  Input: Queue to be initalize
 Output: None
*/
void InitializeQueue(Queue *Q)
{
	// Initialise its properties 
	Q->size = 0;
	Q->capacity = QUEUE_LENGTH;
	Q->front = 0;
	Q->back = -1;
	msDELAY(100);
}

