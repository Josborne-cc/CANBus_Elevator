/*
Purpose: Functions for using the SCI module
Author:  Josh Osborne
Date:    April 6, 2015
Version: 00
*/

#include <hidef.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "TimerModule.h"
#include "SCIModule.h"
#include "utils.h"
#include <stdarg.h>

volatile char SCI_Rx_BUF[SCI_Rx_BUF_LENGTH] = {0};
volatile unsigned int SCI_Rx_INDEX = 0;
int CMD_FLAG;
int COMM_ERROR;
int CMD_START;
unsigned long MESSAGE_COUNT;
volatile unsigned char Serial_CMD[SERIAL_QUEUE_DATA];
volatile unsigned char Serial_Recieve[SERIAL_QUEUE_DATA];
Serial_Queue Serial_In;
int Serial_Count = 0;




/*************SCIintit()*******************************************************
Purpose: Initalize the SCI module
Input:   int BAUD_RATE - a divisor to get the desired baud rate
         int SCI_DATA  - a value that determines parity and data bits
         int CONFIG    - a value that specifies what features the the module will be used
******************************************************************************/
void SCIinit(int BAUD_RATE, int SCI_DATA, int CONFIG) 
{
  SCIBD = BAUD_RATE;
  SCICR1 = LOW(SCI_DATA);
  SCICR2 = LOW(CONFIG);
}

/************putcSCI()*********************************************************
Purpose: Put a character to the Serial Port
Input:   char cx - character to be printed
******************************************************************************/
void putcSCI(char cx) 
{
  while(!(SCISR1&SCISR1_TDRE_MASK));
  SCIDRL = cx;
}

/***********putsSCI()**********************************************************
Purpose: Put a string to the serial port
Input:   char *str - pointer to the printed string
******************************************************************************/
void putsSCI(char *str) 
{
  while(*str)
    putcSCI(*str++);
}

/*

*/
void SCIprintf(char *format, ...)
{
    va_list myArgs;
    char Buffer[32];
    va_start(myArgs, format);
    (void)vsprintf(Buffer,format,myArgs);
    va_end(myArgs);
    putsSCI(Buffer);
}

/*
*/
void getsSCI(int num, char *str) 
{
  int count;
  
  for(count = 0; count < num; count++)
     str[count] = SCI_Rx_BUF[(SCI_Rx_INDEX - num + count)];
     
  str[num] = '\0'; 
}

/*************getcSCI()********************************************************
Purpose: Get a character from the serial port
Input:   char cx - character that is grabed
******************************************************************************/
void getcSCI(char cx) 
{
     if(CMD_FLAG == 1 && cx == '<')
     {
          COMM_ERROR = 1;
     }

     if(cx == '<')
     {
          CMD_START = SCI_Rx_INDEX;
          CMD_FLAG = 1;
     }


     if(CMD_FLAG == 1) 
     {  
          //SCI_Rx_BUF[SCI_Rx_INDEX] = cx;
         // SCI_Rx_INDEX = (SCI_Rx_INDEX + 1) % SCI_Rx_BUF_LENGTH;
           *(SCI_Rx_BUF + (SCI_Rx_INDEX++)) = cx;
           SCI_Rx_INDEX %= SCI_Rx_BUF_LENGTH;
     }
     
     if(cx == '>')
     {
          MESSAGE_COUNT++;
          CMD_FLAG = 0;
     }

}

void SCIgetc(char buf)
{
  *(SCI_Rx_BUF + (SCI_Rx_INDEX++)) = buf;
  SCI_Rx_INDEX %= SCI_Rx_BUF_LENGTH;
}

/************SCI_HANDLER()*****************************************************
Purpose: Grab a character from the serial port on every interupt
******************************************************************************/
interrupt VectorNumber_Vsci void SCI_HANDLER( void )
{
  
  
  // If data is available on port
  if(SCISR1 & SCISR1_RDRF_MASK)
  {
    //getcSCI(SCIDRL);
    Serial_CMD[Serial_Count] = SCIDRL;
    Serial_Count++;
    
    if(Serial_Count == SERIAL_QUEUE_DATA)
    {
        Serial_AddToQueue(&Serial_In);
        Serial_Count = 0;   
    }
    
  }
}

/*************Serial_Create_Queue()*******************************************
Purpose: Create a Serial Queue to store incoming messages
******************************************************************************/
void Serial_Create_Queue(Serial_Queue *Q)
{
   	// Initialise its properties 
	Q->size = 0;
	Q->capacity = SERIAL_QUEUE_LENGTH;
	Q->front = 0;
	Q->rear = -1;
	msDELAY(10); 
}

/************Serial_Dequeue()*************************************************
Purpose: Remove an item from the serial queue
******************************************************************************/
int Serial_Dequeue(Serial_Queue *Q)
{
    int i;
	// If Queue size is zero then it is empty. So we cannot pop 
	if(Q->size==0)
	{
		return -1;
	}
	else
	{
		//Store the data to use
		DisableInterrupts;
		for(i = 0; i < SERIAL_QUEUE_DATA; i++)
		  Serial_Recieve[i] = Q->elements[Q->front][i];
		EnableInterrupts;
		
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

/**************Serial_AddToQueue()********************************************
Purpose: Add an item to the serial queue
******************************************************************************/
//add node to the end of queue
int  Serial_AddToQueue(Serial_Queue *Q)
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
		Q->rear = Q->rear + 1;

		// Fill as a ring buffer
		if(Q->rear == Q->capacity)
		{
			Q->rear = 0;
		}

		// Insert element at the back
		DisableInterrupts;
		for(i = 0; i < SERIAL_QUEUE_DATA; i++) 
		{
			Q->elements[Q->rear][i] = Serial_CMD[i];
		}
		EnableInterrupts;

	}
	return 0;
}

/************Serial_IsQueueEmpty()********************************************
Purpose: Check to see if the serial queue is empty
******************************************************************************/
// Returns 0 if queue is empty
int  Serial_IsQueueEmpty(Serial_Queue Q)
{ 
	int length;

	length = Q.size;

	return length;
}

