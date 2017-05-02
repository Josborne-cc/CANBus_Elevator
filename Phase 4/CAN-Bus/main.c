/*
Purpose: Elevator Controller
 Author: Josh Osborne
   Date: July 23
Version: 4
*/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "utils.h"
#include "TimerModule.h"
#include "LCDModule.h"
#include "SCIModule.h"
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include "USensor.h"
#include <stdlib.h>



#define DAC_ON  0b1101110000000000
#define DAC_OFF 0b1110110000000000




void main(void) 
{

	

    // Set the prescaler
	SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	TSCR1 = TSCR1_INIT;

    // Initalize the modules
	CANinit(CONTROLLER);
	LCDinit();
	SCIinit(BAUD_RATE_DIVISOR_9600, N81_DATA, (SCICR2_RE_MASK|SCICR2_TE_MASK|SCICR2_RIE_MASK));
	LCDclear();

	while (!(CANCTL0&0x10));

	CANRFLG = 0xC3;
	CANRIER = 0x01;

	InitializeQueue(&rec);	// Create the Recieve Queue
	Serial_Create_Queue(&Serial_In);
	Queue_Flag = 0;
	
	

	EnableInterrupts;

	for(;;) 
	{
        // For the Can System
		if(IsQueueEmpty(rec) != 0) 
		{
			(void)DeQueue(&rec);
			Parse_Data();
		}
		
		// From the Serial Link
		if(Serial_IsQueueEmpty(Serial_In) != 0)
		{
		    (void)Serial_Dequeue(&Serial_In);
		    LCDprintf("%s \r", Serial_Recieve); 
		    Serial_Parse();  
		}

		msDELAY(100);
		//_FEED_COP(); /* feeds the dog */
    }

}/* loop forever */

