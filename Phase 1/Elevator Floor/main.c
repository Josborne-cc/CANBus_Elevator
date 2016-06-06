#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "utils.h"
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include "TimerModule.h"

 

static unsigned int data[DATA_LENGTH];

void main(void) 
{
  /* put your own code here */
  
  SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	TSCR1 = TSCR1_INIT;
  
	seg_init();
	CANinit(THE_FLOOR);
	SET_BITS(LED_DDR, LED1_MASK|LED2_MASK);

	node_id = PORTB & 3; //get hardware specified node id
	
	while (!(CANCTL0&0x10));

	CANRFLG = 0xC3;
	CANRIER = 0x01;

	InitializeQueue(&rec);	// Create the Recieve Queue
	
	
	
     clear_seg();

	EnableInterrupts;

  for(;;) 
  {

	if(IsQueueEmpty(rec) != 0)
	{		
		DeQueue(&rec);
		Parse_Floor();
	}
  
  // button 1 press
  if((PTJ & (SWITCH1_MASK |SWITCH2_MASK) )== SWITCH1_MASK) 
  {
    if (last != SWITCH1_MASK) 
	{   // check if button already pressed 

		data[0] = THE_FLOOR;
		data[1] = CALL_SWITCH;
		data[2] = ON;
		
		CANSend(CONTROLLER, 0x00, 0x00, DATA_LENGTH, data); 
		last =  SWITCH1_MASK;        
    }
  }                
  
  //button 2 press
  /*else if((PTJ & (SWITCH1_MASK |SWITCH2_MASK) )== SWITCH2_MASK){
        if (last != SWITCH2_MASK) { // check if button already pressed    
    i=i + node_id;
    
    //led = LED2_MASK;           // do the thing
    
    
    last =  SWITCH2_MASK;        
    }
  } */
  
  else
  last = 0;
  
  
  // Updates the LED
  FORCE_BITS(PTS,0b00001100 , led) ;


  

  } /* loop forever */
  /* please make sure that you never leave main */
}


