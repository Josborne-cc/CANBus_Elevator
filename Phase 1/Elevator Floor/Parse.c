/*
Purpose: Parse the CAN input on the floor nodes
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

volatile unsigned int rxdata[DATA_LENGTH];

int segval[] = {SEG_0,SEG_1,SEG_2,SEG_3,SEG_4,SEG_5,SEG_6,SEG_7,SEG_8,SEG_9}  ;

int count =0;
int i=0;
int node_id =0;

int led = 0;
int last;

/*
!!!!! This funtion is designed for the floor nodes !!!!!

Purpose: Determine what to do with the leds when the can bus code arrives
*/
void Parse_Floor(void) 
{

	// Local variables for assigning the bytes too
	unsigned int node;
	unsigned int sub_comp;
	unsigned int instruction;
  // Grab the three bytes and sort according to their representation
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	if(node == THE_FLOOR) 
	{
		switch(sub_comp)
		{
			case CALL_LED:
				if(instruction == ON)
					led = LED1_MASK;			// Turn the LED on
				else if(instruction == OFF)
					led = 0x00; 				// Turn the LED off
				break;
			case FLOOR_INDICATOR:
				set_seg(instruction - 0x02);			// minus two because of the constants
				break;
		}

	}
}

// Initialize the Seven Segment Display
void seg_init(void) 
{
   
   SET_BITS(SEG_DDR, 0xFFu);

}

/*
Purpose: Write a value to the seven segment display
*/
void set_seg(unsigned int i)
{
  i= i & 0xF;
  
  FORCE_BITS(SEG_PORT, 0xFFu, segval[i]);
}

/*
Purpose: Clear the seven segment display
*/
void clear_seg(void)
{
  FORCE_BITS(SEG_PORT, 0xFFu, SEG_BLANK);
}