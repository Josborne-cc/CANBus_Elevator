/*
Purpose: Parse functions for the elevator car node
 Author: Josh Osborne
   Date: June 6
Version: 1
*/

#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include "TimerModule.h"
#include "LCDModule.h"
#include "utils.h"

/*
Purpose: Determine actions needed from CAN input
*/
void Parse_Car(void)
{
	unsigned int node;
	unsigned int sub_comp;
	unsigned int instruction;
	
	static unsigned int data[DATA_LENGTH];
	
	node = rxdata[0];
	sub_comp = rxdata[1];
	instruction = rxdata[2];
	
	if(node == CAR)		
	{
        switch(sub_comp)
        {
            case DOOR:
                switch(instruction)
                {
                    
                        case 0:
                    	    FORCE_BITS(LED_PORT, LED2, LED2);
                            break;
                        case 1:
				            FORCE_BITS(LED_PORT, LED2, 0x00);
			            	break;
                }
				//msDELAY(2000);
			
                break;
            case FLOOR_INDICATOR:
                switch(instruction)
                {
                    case F1:
                        LCDprintf("Floor 1 \r");
                        msDELAY(100);
                        break;
                    case F2:
                        LCDprintf("Floor 2 \r");
                        break;
                    case F3:
                        LCDprintf("Floor 3 \r");
                        break;
                }
                break;
            case BUTTON_INDICATOR_ON:
                switch(instruction)
                {
                    case F1:
                        FORCE_BITS(LED_PORT, LED3, 0x00); // Turn on the LED
                        break;
                    case F2:
                        FORCE_BITS(LED_PORT, LED4, 0x00); // Turn on the LED
                        break;
                    case F3:
                        FORCE_BITS(LED_PORT, LED5, 0x00); // Turn on the LED
                        break;
                }
                break;
			case BUTTON_INDICATOR_OFF:
                switch(instruction)
                {
                    case F1:
                        FORCE_BITS(LED_PORT, LED3, LED3); // Turn off the LED
                        break;
                    case F2:
                        FORCE_BITS(LED_PORT, LED4, LED4); // Turn off the LED
                        break;
                    case F3:
                        FORCE_BITS(LED_PORT, LED5, LED5); // Turn off the LED
                        break;
                }
                break;
        }
	}
}