/*
Author: Josh Osborne
*/

#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Queue_C.h"
#include "TimerModule.h"
#include "utils.h"





// Returns 0 if queue is empty
int  IsQueueEmpty(Queue Q)
{ 
	int length;

	length = Q.size;

	return length;
}

//add node to the end of queue
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

// Delete node from head of queue
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
	
	Queue_Flag = 1;
	
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

/*
!!!! This function is designed for the controller !!!!!

Purpose: Parse the message into each of the components and then distrubute the commands as nessesary
  Input: Recieve Queue
 Output: none
*/
void Parse_Data(void)
{
	// Local variables for assigning the bytes too
	unsigned int node;
	unsigned int sub_comp;
	unsigned int instruction;
	
	Queue_Flag = 0;  // Says that instruction has been delt with
	
	// Grab the three bytes and sort according to their representation
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	switch (node)
	{
		// PID Controller
		case PID:
			//Feedback(sub_comp, instruction);
			break;
		// Elevator Car
		case CAR:
			//Car(sub_comp, instruction);
			break;
		// Floor 1
		case F1:
			Floor(F1, sub_comp, instruction);
			break;
		// Floor 2
		case F2:
			break;
		// Floor 3
		case F3:
			break;
	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: To send the commands to each floor node as needed
*/
void Floor(unsigned int floor, unsigned int sub_comp, unsigned int instruction)
{
	static unsigned int data[DATA_LENGTH];
	
	if(sub_comp == CALL_SWITCH)
	{
		data[0] = floor;
		data[1] = CALL_LED;
		data[2] = ON;
		CANSend(0xFF, 0x00, DATA_LENGTH, data);
	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: Send out instructions from the PID
*/
/*void Feedback(unsigned int sub_comp, unsigned int instruction)
{
	static int data[DATA_LENGTH];
	
	if(sub_comp == REC_POINT)		// Check to make sure that processing is correct
	{
		// Distribute the floor to each node
	}
}*/

/*
!!!! This function is designed for the controller !!!!!

Purpose: Send out instructions from the elevator car node
  Input: Requests from the car node
*/
/*void Car(unsigned int sub_comp, unsigned int instruction)
{
	static int data[DATA_LENGTH];
	
	switch(sub_comp)
	{
		// Floor button
		case FLOOR_BUTTON:
			// From the elevator car node to the disance sensor
			data[2] = PID;
			data[1] = SET_POINT;
			data[0] = instruction;
			CANSend(id, priority, DATA_LENGTH, data);
			// From the car back to the car
			data[2] = CAR;
			data[1] = BUTTON_INDICATOR;
			data[0] = instruction;
			CANSend(id, priority, DATA_LENGTH, data);
			break;
		// Door
		case DOOR:
			// From the car back to the car
			data[2] = CAR;
			data[1] = DOOR;
			data[0] = instruction ^= 0x01; // Flips the last bit
			CANSend(id, priority, DATA_LENGTH, data);
			break;
		// E-Stop
		case E_STOP:
			break;
	}
}*/
