/*
Purpose: Parse functions for the controller node
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

/*
!!!! This function is designed for the controller !!!!!

Purpose: Parse the message into each of the components and then distrubute the commands as nessesary
  Input: Recieve Queue
 Output: none
*/
void Parse_Data(void)
{
	// Local variables for assigning the bytes too
	unsigned int node;
	unsigned int sub_comp;
	unsigned int instruction;
	
	// Grab the three bytes and sort according to their representation
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	switch (node)
	{
		// PID Controller
		case PID:
			Feedback(sub_comp, instruction);
			break;
		// Elevator Car
		case CAR:
			Car(sub_comp, instruction);
			break;
		// Floor 1
		case F1:
			Floor(F1, sub_comp, instruction);
			break;
		// Floor 2
		case F2:
		    Floor(F2, sub_comp, instruction);
			break;
		// Floor 3
		case F3:
		    Floor(F3, sub_comp, instruction);
			break;
	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: To send the commands to each floor node as needed
*/
void Floor(unsigned int floor, unsigned int sub_comp, unsigned int instruction)
{
	static unsigned int data[DATA_LENGTH];
	
	if(sub_comp == CALL_SWITCH)
	{
	    // Send led light so the user knows the car is coming
		data[0] = floor;
		data[1] = CALL_LED;
		data[2] = ON;
		CANSend(floor, 0x00, 0x00, DATA_LENGTH, data);
		
		// Send Setpoint to PID so the car goes to that floor
		data[0] = PID;
		data[1] = SET_POINT;
		data[2] = floor;
		CANSend(floor, 0x00, 0x00, DATA_LENGTH, data);
	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: Send out instructions from the PID
  Input: sub_comp - should always be REC_POINT
         instruction - should be the floor level
*/
void Feedback(unsigned int sub_comp, unsigned int instruction)
{
	static unsigned int data[DATA_LENGTH];
	
	if(sub_comp == REC_POINT)		// Check to make sure that processing is correct
	{
		// Distribute the floor indicator to each node
		
			data[0] = CAR;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			data[0] = F1;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			CANSend(F1, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			data[0] = F2;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			CANSend(F2, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			data[0] = F3;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			CANSend(F3, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			//Shut off the call led
			data[0] = instruction;
			data[1] = CALL_LED;
			data[2] = OFF;
			CANSend(instruction, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			// Shut off elevator car led
			data[0] = CAR;
			data[1] = BUTTON_INDICATOR_OFF;
			data[2] = instruction;
			CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			// turn on the car door led
			data[0] = CAR;
			data[1] = DOOR;
			data[2] = OPEN;
			CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			

	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: Send out instructions from the elevator car node
  Input: Requests from the car node
*/
void Car(unsigned int sub_comp, unsigned int instruction)
{
	static unsigned int data[DATA_LENGTH];
	
	switch(sub_comp)
	{
		// Floor button
		case FLOOR_BUTTON:
			// From the elevator car node to the disance sensor
			data[0] = PID;
			data[1] = SET_POINT;
			data[2] = instruction;
			CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);
			
			// From the car back to the car
			data[0] = CAR;
			data[1] = BUTTON_INDICATOR_ON;
			data[2] = instruction;
			CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			
		// E-Stop
		case E_STOP:
			break;
	}
}
