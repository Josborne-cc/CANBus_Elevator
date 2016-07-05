/*
Author: Josh Osborne and Bryan Peng
*/

#include <hidef.h>      /* common defines and macros */
#include <mc9s12c32.h>        /* derivative information */
#include "derivative.h"      /* derivative-specific definitions */
#include <stdio.h>
#include <stdlib.h>
#include "LCDModule.h"
#include "TimerModule.h"
#include "utils.h"
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
 

int last =0;


void main(void) 
{

    static unsigned char data[DATA_LENGTH];

    // TSCR2 = TSCR2_PRESCALE_8;       // set prescaler to 8, no interrupt on TOF
    SET_TCNT_PRESCALE( TCNT_PRESCALE_8 ); 

    // Enable TCNT, fast timer flag clear, freeze
    TSCR1 = TSCR1_INIT;

    LCDinit();
    CANinit(CAR);
    LCDclear();
    

    while (!(CANCTL0&0x10));

    CANRFLG = 0xC3;
    CANRIER = 0x01;

    SET_BITS(LED_DDR, 0xFFu);
    
    // Turn all the LEDs off 
    FORCE_BITS(LED_PORT, 0xFF, 0xFF);
	
	SET_BITS(LED1_DDR, LED1_MASK|LED2_MASK);
    
    InitializeQueue(&rec);	// Create the Recieve Queue
    
    //LCDprintf("josh");

    EnableInterrupts;


    for(;;) 
    {
    
        if(IsQueueEmpty(rec) != 0) 
        {
            DeQueue(&rec);
            Parse_Car();
        }


        switch(SWITCH_PORT)  
        {


        case 1:

            if (last != SWITCH0) 
            {   // check if button already pressed 

                FLIP_BITS(LED_PORT, LED0) ;
                last =  SWITCH0;
                        
            }
            break;

        // Door Close
        case 2:

            if (last != SWITCH1) 
            {   // check if button already pressed 

                //FLIP_BITS(LED_PORT, LED1) ;
                last =  SWITCH1;
                FORCE_BITS(LED_PORT, LED2, LED2);
            }
            break;

        // Door Open
        case 4:
            if (last != SWITCH2) 
            {   // check if button already pressed 

                //FLIP_BITS(LED_PORT, LED2);
                last =  SWITCH2;
                FORCE_BITS(LED_PORT, LED2, 0x00);
                       
            }
            break;

        // Floor 1 
        case 8:
            if (last != SWITCH3) 
            {   // check if button already pressed 

                //FLIP_BITS(LED_PORT, LED3);
                last =  SWITCH3;
                
                data[0] = CAR;
                data[1] = FLOOR_BUTTON;
                data[2] = F1;
		
		        CANSend(F1, 0x00, 0x00, DATA_LENGTH, data);         
            }
            break;
            
        // Floor 2
        case 16:
            if (last != SWITCH4) 
            {   // check if button already pressed 

                //FLIP_BITS(LED_PORT, LED4) ;
                last =  SWITCH4;
                
                data[0] = CAR;
                data[1] = FLOOR_BUTTON;
                data[2] = F2;
		
		        CANSend(F2, 0x00, 0x00, DATA_LENGTH, data);         
        
            }
            break;
        
        // Floor 3
        case 32  :

            if (last != SWITCH5) 
            {   // check if button already pressed 

                //FLIP_BITS(LED_PORT, LED5) ;
                last =  SWITCH5;
                
                data[0] = CAR;
                data[1] = FLOOR_BUTTON;
                data[2] = F3;
		
		        CANSend(F3, 0x00, 0x00, DATA_LENGTH, data);          
            }
            break;

        case 64:

            if (last != SWITCH6) 
            {   // check if button already pressed 

                FLIP_BITS(LED_PORT, LED6);
                last =  SWITCH6;        
            }
            break;

        default :
        last = 0;

        }


    // _FEED_COP(); /* feeds the dog */
    } /* loop forever */
      /* please make sure that you never leave main */
}
