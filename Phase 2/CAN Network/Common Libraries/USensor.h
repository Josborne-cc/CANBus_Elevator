/*********************************************
Bryan Peng 2016
HCS12 driver code header file for HC-SR04 ultrasonic sensor
*********************************************/

#ifndef _USensor_H
#define _USensor_H

#define HIGH_            1
#define LOW_             0
#define UpperLimit    7550  // Upper Limit of valid on time
#define LowerLimit    2499  // Lower Limit of valid on time
#define WatchDogTime 10000  // Watch Dog timer
#define WatchDogTime 10000  // Watch Dog timer

//Globals
extern unsigned long PULSE_WIDTH;
extern volatile int STATE;
extern volatile int INPUT;
extern volatile int ONTIME;
extern volatile int PERIOD;
extern volatile int IN_STATE;
extern int i;
extern int distance;

// Watchdog timer flags
extern int WATCH_DOG_FLAG;
extern int WATCH_DOG_INPUT;   // Set when input capture interupt is triggered

// Flags values
enum flag 
{
  INVALID,
  VALID
};

// Used to signify which edge the input state will be in
enum edge
{
  FIRST_RISING,
  RISING,
  FALLING
};

void initDistance(void);
void getDistance(void);
long convert(int timeDifference);
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 0 ) ) void TC0_OC_handler( void );
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 1 ) ) void TC1_IC_handler( void );
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 2 ) ) void TC2_OC_handler( void );

#endif