/*
Author: Josh Osborne
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "utils.h"
#include "TimerModule.h"
#include "LCDModule.h"
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include <stdlib.h>




void main(void) 
{
	int i;
	//static unsigned int test[DATA_LENGTH];

	SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	TSCR1 = TSCR1_INIT;

	//SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	//TSCR1 = TSCR1_INIT;
	CANinit(CONTROLLER);
	LCDinit();

	while (!(CANCTL0&0x10));

	CANRFLG = 0xC3;
	CANRIER = 0x01;

	InitializeQueue(&rec);	// Create the Recieve Queue
	Queue_Flag = 0;
	
	// Test values for transmiting
	txbuf[0] = 0x12;
	txbuf[1] = 0x34;
	

     
     //test[0] = 0;
     //test[1] = 0;
     //test[2] = 0;

	EnableInterrupts;

	for(;;) 
	{
		//CANSend(0xFF, 0x00, sizeof(txbuf)-1, txbuf);
		
	/*	for(i = 0; i < 30; i++)
		{
		    test[2] = i;
		    CANSend(0xFF, 0x00, DATA_LENGTH, test);
		}*/
		    

		// Add data to queue
	/*	if(Queue_Flag == 1)
		{
			AddToQueue(&rec); 
			Queue_Flag = 0;
		}*/

		if(IsQueueEmpty(rec) != 0) 
		{
		  
			DeQueue(&rec);
			Parse_Data();
		}
		
		//if(Parse_Flag == 1)
	  //	Parse_Data();

		for(i = 0; i < DATA_LENGTH; i++) 
		{
			LCDprintf("%02X", rxdata[i]);
			//LCDprintf("%x", DeQueue(&rec));
		}
			LCDprintf("\r");

		msDELAY(100);
		//_FEED_COP(); /* feeds the dog */
}

}/* loop forever */



