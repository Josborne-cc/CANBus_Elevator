/*
Purpose: Moves the elevator
 Author: Josh Osborne
   Date: July 23
Version: 2
*/

#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include "TimerModule.h"
#include "utils.h"

int dest_floor = 0;

/*
Purpose: Takes the recieve point and delays then sends that recieve point back out saying the elevator arrived at the setpoint 
*/
void Parse_PID(void)
{
	// Local variables for assigning the bytes too
	unsigned char node;
	volatile unsigned char sub_comp;
	unsigned char instruction;
	
	// Grab the three bytes and sort according to their representation
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	  if(sub_comp == SET_POINT)
	    dest_floor = instruction;
	
	/*
	if(node == PID && sub_comp == SET_POINT)		
	{
		// Send back that the PID has moved the car to the requested floor
		data[0] = PID;
		data[1] = REC_POINT;
		data[2] = instruction;
		CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);
	}
	*/
}