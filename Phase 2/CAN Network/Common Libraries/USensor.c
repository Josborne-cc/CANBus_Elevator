/*********************************************
Bryan Peng 2016
HCS12 driver code for HC-SR04 ultrasonic sensor
*********************************************/

#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include<stdio.h>
#include<stdlib.h>
#include "utils.h"
#include "TimerModule.h"
#include "LCDModule.h"
#include "USensor.h"


//Globals
unsigned long PULSE_WIDTH;
volatile int STATE;
volatile int INPUT;
volatile int ONTIME;
volatile int PERIOD;
volatile int IN_STATE;
int i;
int distance;

// Watchdog timer flags
int WATCH_DOG_FLAG;
int WATCH_DOG_INPUT;   // Set when input capture interupt is triggered


/***************************************************
 
 initialization for code
 sets up intterupt vectors and specifys ports for sensor

****************************************************/
void initDistance() {

     SET_BITS(DDRB,0xff);

      // Set channel 0 to output capture
  MAKE_CHNL_OC( 0 );
  
  // Set channel 1 to input capture
  MAKE_CHNL_IC( 1 );
  
  // Set channel 2 to output compare for watchdog
  MAKE_CHNL_OC( 2 );
  

  
  // Set input capture for first rising edge
  SET_IC_EDGE(1, IC_EDGE_RISING);
	IN_STATE = FIRST_RISING;
  
  // preset TC0 for first OC event
  TIMER_CHNL( 0 ) = TCNT + (unsigned int)PULSE_WIDTH;
  
  // preset TC2 for first OC event
  TIMER_CHNL( 2 ) = TCNT + 10000;  

  //Enable the interrupts on channel 0 and 1
  ENABLE_CHNL_INTERRUPT( 0 );
  ENABLE_CHNL_INTERRUPT( 1 );
  ENABLE_CHNL_INTERRUPT( 2 );
  
  //Clear any pending interrupts on channel 0
  TFLG1 = (TFLG1_C0F_MASK | TFLG1_C1F_MASK);
}

void getDistance(){

CLR_BITS(PORTB, 0x01) ;
FLIP_BITS(PORTB, 0x01);
msDELAY(10);
FLIP_BITS(PORTB, 0x01);

}

/***************************************************

  convert time measuremnt to distance in centimeters
  
****************************************************/  
long convert(int timeDifference) {
  int distance = timeDifference / 58; // divisor value specified in HC-SR04 module
  return distance;
}


//Interrupt vector for sensor control
/**************************Output Compare Handler()****************************
sends a 20us pulse every 30 ms
******************************************************************************/
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 0 ) ) void TC0_OC_handler( void )
{
  static int lowcount;
  TFLG1 = TFLG1_C0F_MASK;
  
 
    
  switch(STATE) 
  {
     case LOW_:
     if(lowcount < 1)      {
      
     lowcount ++;
      TC0 += 30000;  // Remainder of period is low time      change this value to adjust time between pulses
     }
     else{
      
     SET_OC_ACTION(0, OC_GO_HI);    // Change the state of the waveform
     STATE = HIGH_;                  // Set the State indicator
     TC0 += 30000;  // Remainder of period is low time
     lowcount =0;
     }
     break;
     
     case HIGH_:
     SET_OC_ACTION(0, OC_GO_LO);    // Change the state of the waveform
     STATE = LOW_;                  // Set the State indicator
     TC0 += 20;            // Set High time of period
     break;
  }
 

      
} // end of TC0_OC_handler()

//interrupt vector for calculating pulse width

/***********************Input Capture Handler()********************************
-calculates period
-calculates ontime
-changes diection of next captured edge
******************************************************************************/                                                        
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 1 ) ) void TC1_IC_handler( void ) 
{
  static int EDGE_1;
  static int EDGE_2;
  //int period_temp;
  
  // Clear the timer flag
  //TFLG1 = TIMER_CHNL_FLAG( 1 );
  TFLG1 = TFLG1_C1F_MASK;
  
  // Tell the watchdog that there was valid input
  WATCH_DOG_INPUT = VALID;
  
  // Record the time of the first rising edge
  if(IN_STATE == FIRST_RISING)
  {
    EDGE_1 = TC1;                                 // Record the time stamp
    IN_STATE = FALLING;               
    SET_IC_EDGE(1, IC_EDGE_FALLING);              // Change the edge type for next input
  }
  
  // Gather rising edge time stamp 
  else if(IN_STATE == RISING)
  {
    PERIOD = EDGE_1 - TC1;
    ONTIME = EDGE_2 - EDGE_1;      
    IN_STATE = FALLING;
    SET_IC_EDGE(1, IC_EDGE_FALLING);
    EDGE_1 = TC1;   
  }
  
  // Gather falling edge time stamp 
  else if(IN_STATE == FALLING) 
  {
    IN_STATE = RISING;
    SET_IC_EDGE(1, IC_EDGE_RISING);               // Change the edge type for next input
    EDGE_2 = TC1;                                 // Save the edge time
  }
  
}  // end of TC1_IC_handler()


/******************************Watchdog Handler()*************************
-checks the watchdog flag for valid data
*************************************************************************/
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 2 ) ) void TC2_OC_handler( void ) 
{
  // Clear the interrupt flag 
  TFLG1 = TFLG1_C2F_MASK;

  if(WATCH_DOG_INPUT == INVALID)
    WATCH_DOG_FLAG = INVALID;
  else
    WATCH_DOG_FLAG = VALID;
  
  TIMER_CHNL( 2 ) = TCNT + WatchDogTime; // Feed the dog
}
      