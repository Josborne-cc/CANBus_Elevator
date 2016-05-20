#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "utils.h"
#include "TimerModule.h"
#include "LCDModule.h"

void CANinit(void);
interrupt 38 void RxISR(void);
unsigned char CANSend(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata);

volatile unsigned char rxdata[8];

void main(void) 
{


  //unsigned char errorflag = NO_ERR;
  unsigned char txbuff[] = "Josh";
  
  SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
  TSCR1 = TSCR1_INIT;
  
	//SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	//TSCR1 = TSCR1_INIT;
	CANinit();
	LCDinit();
	
	while (!(CANCTL0&0x10));
	
	CANRFLG = 0xC3;
	CANRIER = 0x01;
	

	EnableInterrupts;

	for(;;) 
	{
    CANSend(0xFF, 0x00, sizeof(txbuff)-1, txbuff);
	    LCDprintf("%s", rxdata);
	msDELAY(100);
	//_FEED_COP(); /* feeds the dog */
}

}/* loop forever */

void CANinit (void)
{
     //CANCTL1	|= 0x80;   // enable CAN, required after reset
     SET_BITS(CANCTL1, CANCTL1_CANE_MASK);
     //CANCTL0  |= 0x01; // request to enter initialization mode
     SET_BITS(CANCTL0, CANCTL0_INITRQ_MASK);
	 
     while(!(CANCTL1&0x01)) {}; // wait until initialization mode is entered
	 
     CANCTL1   = 0x84;    // enable CAN1, select oscillator as MSCAN clock
                           
     
     CANBTR0   = 0x03;    // set SJW to 2, set prescaler to 8
     CANBTR1   = 0x32;    
         
	// MSCAN Acceptance Registers - Accept All
     CANIDAR0  = 0x00;   
     CANIDAR1  = 0x00;   
     CANIDAR2  = 0x00;   
     CANIDAR3  = 0x00;   
     CANIDAR4  = 0x00;   
     CANIDAR5  = 0x00;
     CANIDAR6  = 0x00;  
     CANIDAR7  = 0x00; 
     
     CANIDMR0  = 0xFF;   
     CANIDMR1  = 0xFF;  
     CANIDMR2  = 0xFF;  
     CANIDMR3  = 0xFF;	
     CANIDMR4  = 0xFF;   
     CANIDMR5  = 0xFF;   
     CANIDMR6  = 0xFF; 
     CANIDMR7  = 0xFF;   
     
     CANIDAC   = 0x00;   /* select two 32-bit filter mode */
     
     //CLR_BITS(CANCTL0, CANCTL0_INITRQ_MASK); /* exit initialization mode */
     CANCTL0 &=  ~CANCTL0_INITRQ_MASK;
	 
	   while((CANCTL1&0x01) != 0) {};
	   CANCTL1 = (CANCTL1_CANE_MASK);
	   CANRIER = (CANRIER_RXFIE_MASK); /// enable interrupts  
     
     //CANCTL0 	= 0x24;    /* stop clock on wait mode,enable wake up */
	 
	 
}


unsigned char CANSend(unsigned long id, unsigned char priority, unsigned char length, unsigned char *txdata) 
{
  int i;
	unsigned char txbuffer = 0;
	
	// Check to see if the transfer buffer is full
	if(!CANTFLG)						
		return 1;

	// Select the lowest empty buffer	
	CANTBSEL = CANTFLG;     

	// Backup the selected buffer
	txbuffer = CANTBSEL;          

	// Load id to IDR register
*((unsigned long *) ((unsigned long)(&CANTXIDR0))) = id;
	
	// Load data to transmit buffer
	for(i = 0; i < length; i++)
	{
		*(&CANTXIDR0 + i) = txdata[i];
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
*/
interrupt 38 void RxISR(void) 
{
  unsigned char length, index;

  
  length = (CANRXDLR & 0x0F);  
  
  for (index=0; index<length; index++)
    rxdata[index] = *(&CANRXDSR0 + index); /* Get received data */
  
  CANRFLG = 0x01; /* Clear RXF */  
}
