/*
	Purpose: Functions for CAN Bus transmissions
	 Author: Josh Osborne
Contributor: Loren Copeland

*/

#include "derivative.h"      /* derivative-specific definitions */
#include "CAN.h"
#include "Queue.h"
#include "TimerModule.h"
#include "utils.h"


// buffers for use with the CAN transmission
volatile unsigned int rxbuf[DATA_LENGTH];
volatile unsigned int txbuf[DATA_LENGTH];

/*
Purpose: Initalize the CAN Bus
  Input: Constant that represents the node being initalized
*/
void CANinit (unsigned char id)
{
	//CANCTL1	|= 0x80;   // enable CAN, required after reset
	SET_BITS(CANCTL1, CANCTL1_CANE_MASK);
	//CANCTL0  |= 0x01; // request to enter initialization mode
	SET_BITS(CANCTL0, CANCTL0_INITRQ_MASK);

	while(!(CANCTL1&0x01)) {}; // wait until initialization mode is entered

	CANCTL1   = 0x84;    // enable CAN1, select oscillator as MSCAN clock


	CANBTR0   = 0x03;    // set SJW to 2, set prescaler to 8
	CANBTR1   = 0x32;    


  //select filters based on node
  switch(id){
    
    case CONTROLLER:
    	// Controller MSCAN Acceptance Registers - Accept All from 0-7
    	CANIDAR0  = 0x00;   
    	CANIDAR1  = 0xE0;    

    	CANIDMR0  = 0x00;   
    	CANIDMR1  = 0xE0;  
    	break;
    
    
    // Other Devices' MSCAN Acceptance Registers - Accept only controller
    case PID:
    case CAR:
    case F1:      
    case F2:
    case F3:
      CANIDAR0  = 0x00;
      CANIDAR1  = 0x00;
      
      CANIDMR0  = 0x00;
      CANIDMR1  = 0xE0;
      break;
  }


  //Unfilled MSCAN Acceptance Registers  
	CANIDAR2  = 0x00;   
	CANIDAR3  = 0x00;   
	CANIDAR4  = 0x00;   
	CANIDAR5  = 0x00;
	CANIDAR6  = 0x00;  
	CANIDAR7  = 0x00;

    CANIDMR2  = 0x00;  
	CANIDMR3  = 0x00;	
	CANIDMR4  = 0x00;   
	CANIDMR5  = 0x00;   
	CANIDMR6  = 0x00; 
	CANIDMR7  = 0x00; 

	CANIDAC   = 0x10;   /* select four 16-bit filter mode */

	//CLR_BITS(CANCTL0, CANCTL0_INITRQ_MASK); /* exit initialization mode */
	CANCTL0 &=  ~CANCTL0_INITRQ_MASK;

	while((CANCTL1&0x01) != 0) {};
	CANCTL1 = (CANCTL1_CANE_MASK);
	CANRIER = (CANRIER_RXFIE_MASK); /// enable interrupts  

	//CANCTL0 	= 0x24;    /* stop clock on wait mode,enable wake up */
	
}

/*
Purpose: Sends the CAN Bus Buffer to the line
  Input: id - id of the node
		 rtr - request for return - currently always zero because it has not been implemented yet
		 priority - the importance of the message, these priorities are based on the priority of the sender
		 length - length of the data node
		 txdata - the message that is being sent across
*/
/*CANSend:
id -  can controller  0
      PID controller  1
      Elevator car    2
      Floor 1         3
      Floor 2         4
      Floor 3         5
*/
unsigned char CANSend(unsigned int id, unsigned char rtr, unsigned char priority, unsigned char length, unsigned int *txdata) 
{
  int i;
	unsigned char txbuffer = 0;
	
	// Check to see if the transfer buffer is full
	if(!CANTFLG)						
		return 1;

	// Select the lowest empty buffer	
	CANTBSEL = CANTFLG;     

	// Backup the selected buffer
	do
	{
	 txbuffer = CANTBSEL;   
	}while(txbuffer == 0);
	          

	// Load id to IDR register
	//*((unsigned long *) ((unsigned long)(&CANTXIDR0))) = id;
	
	//set transmit ID / priority
	CANTXIDR0 = HIGH(id);                       			//set upper 8 bits of standard ID
	FORCE_BITS(CANTXIDR1, 0xE0, LOW(id << 5));   			//set lower 3 bits of standard ID
    FORCE_BITS(CANTXIDR1, 0x10, rtr);           			//set RTR bit	
    CLR_BITS(CANTXIDR1, 0x08);							    //set 11-bit identifier mode  

	
	// Load data to transmit buffer
	for(i = 0; i < length; i++)
	{
		*(&CANTXDSR0 + i) = txdata[i];
	}
	
	// Set the data length
	CANTXDLR = length;
	
	// Set the priority
	CANTXTBPR = priority;
	
	// Start transmission
	CANTFLG = txbuffer;
	
	// Wait for transmission to complete
	while((CANTFLG & txbuffer));
}

/*
Interupt Service Routine for CAN Recieve
Purpose: Grabs the data of the message and stores that in a queue for future processing
*/
interrupt 38 void RxISR(void) 
{
	unsigned char length, index;

	// Determine the length of the message
	length = (CANRXDLR & 0x0F);  

	// Copy the CAN buffer to my buffer 
	for (index=0; index<length; index++)
		rxbuf[index] = *(&CANRXDSR0 + index); /* Get received data */
	
	//CALL THE ADD TO QUEUE FUNTION HERE
	AddToQueue(&rec);

	// Clear the recieve flag for next message
	CANRFLG = 0x01; /* Clear RXF */  
}