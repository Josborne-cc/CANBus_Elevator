/*
Purpose: Simulates the moving of the elevator car only needed for phase 1
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

/*
Purpose: Takes the recieve point and delays then sends that recieve point back out saying the elevator arrived at the setpoint 
*/
void Parse_PID(void)
{
	unsigned int node;
	unsigned int sub_comp;
	unsigned int instruction;
	
	static unsigned int data[DATA_LENGTH];
	
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	if(node == PID && sub_comp == SET_POINT)		
	{
		msDELAY(3000);

		// Send back that the PID has moved the car to the requested floor
		data[0] = PID;
		data[1] = REC_POINT;
		data[2] = instruction;
		CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);
	}
}