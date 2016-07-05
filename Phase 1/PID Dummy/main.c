#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "utils.h"
#include "TimerModule.h"
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include <stdlib.h>



void main(void) {
  /* put your own code here */
  
	int i;
	//static unsigned int test[DATA_LENGTH];

	SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	TSCR1 = TSCR1_INIT;

	//SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	//TSCR1 = TSCR1_INIT;
	CANinit(PID);

	while (!(CANCTL0&0x10));

	CANRFLG = 0xC3;
	CANRIER = 0x01;

	InitializeQueue(&rec);	// Create the Recieve Queue
	
	// Test values for transmiting
	txbuf[0] = 0x12;
	txbuf[1] = 0x34;

	EnableInterrupts;


  for(;;) 
  {
 		if(IsQueueEmpty(rec) != 0) 
		{
			DeQueue(&rec);
			Parse_PID();
		}
		
  } /* loop forever */
  /* please make sure that you never leave main */
}
