/*
Author: Josh Osborne
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




void main(void) 
{
	int last_floor = 0;
	int floor = 0;
	

    // Set the prescaler
	SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	TSCR1 = TSCR1_INIT;

    // Initalize the modules
	CANinit(CONTROLLER);
	LCDinit();
	SCIinit(BAUD_RATE_DIVISOR_9600, N81_DATA, (SCICR2_RE_MASK|SCICR2_TE_MASK|SCICR2_RIE_MASK));
	initDistance();
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
		
		// Form the Serial Link
		if(Serial_IsQueueEmpty(Serial_In) != 0)
		{
		    (void)Serial_Dequeue(&Serial_In);
		    //LCDprintf("%s \r", Serial_Recieve); 
		    Serial_Parse();  
		}
		
        getDistance();
        DisableInterrupts;
        distance = ONTIME;
        EnableInterrupts;

        distance = convert(distance);
        //LCDclear();
        LCDprintf("%d \r", distance);
        
        // Alert that the elevator car has arrived
        if(distance < 10 && distance > 2)
             floor = F1;
        else if(distance < 63 && distance > 56)
            floor = F2;
        else if(distance < 123 && distance > 119)
            floor = F3;
        
        else
            floor = 0;
            
        
        // Say that the floor has changed
        if(floor != last_floor && floor != 0)
        //if(floor != 0)
        {
            Feedback(REC_POINT, floor);
            last_floor = floor;   
        }
            

		msDELAY(100);
		//_FEED_COP(); /* feeds the dog */
}

}/* loop forever */



