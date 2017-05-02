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
#include "SCIModule.h"
#include "utils.h"
#include "USensor.h"






/*
!!!! This function is designed for the controller !!!!!

Purpose: Parse the message into each of the components and then distrubute the commands as nessesary
  Input: Recieve Queue
 Output: none
*/
void Parse_Data(void)
{
	// Local variables for assigning the bytes too
	unsigned char node;
	unsigned char sub_comp;
	unsigned char instruction;
	
	// Grab the three bytes and sort according to their representation
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	SCIprintf("%d%d%d", rxdata[0], rxdata[1], rxdata[2]);
	
	//putsSCI(rxdata);
	
	switch (node)
	{
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
		default:
			    msDELAY(100);
			    break;
	}
}

void Serial_Parse(void)
{
   	// Local variables for assigning the bytes too
	unsigned int node;
	unsigned int sub_comp;
	unsigned int instruction;
	
	// Grab the three bytes and sort according to their representation
	node = Serial_Recieve[0] - 48;
	sub_comp = Serial_Recieve[1] - 48;
	instruction = Serial_Recieve[2] - 48;
	
	switch (node)
	{
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
		default:
			    msDELAY(100);
			    break;
	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: To send the commands to each floor node as needed
*/
void Floor(unsigned char floor, unsigned char sub_comp, unsigned char instruction)
{
	unsigned char data[DATA_LENGTH];
	
	if(sub_comp == CALL_SWITCH1)
	{
	    // Send led light so the user knows the car is coming
		data[0] = floor;
		data[1] = CALL_LED1;
		data[2] = ON;
		SCIprintf("%d%d%d", data[0], data[1], data[2]);
		(void)CANSend(floor, 0x00, 0x00, DATA_LENGTH, data);
		
		switch(floor)
			{
			    case F1:
			        LCDprintf("\nGo To: 1 \r",0);
			        break;
			    case F2:
			        LCDprintf("\nGo To: 2 \r",0);
			        break;
			    case F3:
			        LCDprintf("\nGo To: 3 \r",0);
			        break;
			}
        data[0] = PID;
        data[1] = SET_POINT;
        data[2] = floor;
        SCIprintf("%d%d%d", data[0], data[1], data[2]);
        (void)CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);
			
	}
	else if(sub_comp == CALL_SWITCH2)
	{
        data[0] = floor;
		data[1] = CALL_LED2;
		data[2] = ON;
		SCIprintf("%d%d%d", data[0], data[1], data[2]);
		(void)CANSend(floor, 0x00, 0x00, DATA_LENGTH, data);
		
		switch(floor)
			{
			    case F1:
			        LCDprintf("\nGo To: 1 \r",0);
			        //dest_floor = F1;
			        break;
			    case F2:
			        LCDprintf("\nGo To: 2 \r",0);
			        break;
			    case F3:
			        LCDprintf("\nGo To: 3 \r",0);
			        break;
			}
        data[0] = PID;
        data[1] = SET_POINT;
        data[2] = floor;
        SCIprintf("%d%d%d", data[0], data[1], data[2]);
        (void)CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);	    
	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: Send out instructions from the PID
  Input: sub_comp - should always be REC_POINT
         instruction - should be the floor level
*/
void Feedback(unsigned char sub_comp, unsigned char instruction)
{
	static unsigned char data[DATA_LENGTH];
	
	if(sub_comp == REC_POINT)		// Check to make sure that processing is correct
	{
		// Distribute the floor indicator to each node
		
			data[0] = CAR;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			data[0] = F1;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(F1, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			data[0] = F2;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(F2, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			data[0] = F3;
			data[1] = FLOOR_INDICATOR;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(F3, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			//Shut off the call led
			data[0] = instruction;
			data[1] = CALL_LED1;
			data[2] = OFF;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(instruction, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
		    data[0] = instruction;
			data[1] = CALL_LED2;
			data[2] = OFF;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(instruction, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			// Shut off elevator car led
			data[0] = CAR;
			data[1] = BUTTON_INDICATOR_OFF;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			
			msDELAY(100);
			
			// turn on the car door led
		/*	data[0] = CAR;
			data[1] = DOOR;
			data[2] = OPEN;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data); */
			

	}
}

/*
!!!! This function is designed for the controller !!!!!

Purpose: Send out instructions from the elevator car node
  Input: Requests from the car node
*/
void Car(unsigned char sub_comp, unsigned char instruction)
{
	static unsigned char data[DATA_LENGTH];
	
	switch(sub_comp)
	{
		// Floor button
		case FLOOR_BUTTON:
			// From the elevator car node to the disance sensor
			/*data[0] = PID;
			data[1] = SET_POINT;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);*/
			
			// From the car back to the car
			data[0] = CAR;
			data[1] = BUTTON_INDICATOR_ON;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);
			
    		switch(instruction)
			{
			    case 3:
			        LCDprintf("\nGo To: 1 \r",0);
			        break;
			    case 4:
			        LCDprintf("\nGo To: 2 \r",0);
			        break;
			    case 5:
			        LCDprintf("\nGo To: 3 \r",0);
			        break;
			}
			break;
	    case 1:
	        data[0] = CAR;
			data[1] = 1;
			data[2] = instruction;
			SCIprintf("%d%d%d", data[0], data[1], data[2]);
			(void)CANSend(CAR, 0x00, 0x00, DATA_LENGTH, data);		
			break;
			
		// E-Stop
		case E_STOP:
			break;
	}
}
