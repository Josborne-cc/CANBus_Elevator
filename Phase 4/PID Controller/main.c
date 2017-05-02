/*
/*
Purpose: Moves the elevator
 Author: Josh Osborne
   Date: July 23
Version: 2
*/


#include <hidef.h>      /* common defines and macros */
#include "derivative.h"      /* derivative-specific definitions */
#include "utils.h"
#include "TimerModule.h"
#include "LCDModule.h"
#include "CAN.h"
#include "Queue.h"
#include "Parse.h"
#include "USensor.h"
#include <stdlib.h>

#define SPICR1_INIT 0b01010000
#define SPI_DIV 0x02;

#define SPI_CS_PORT PTJ
#define SPI_CS_PORT_DDR DDRJ
#define SPI_CS1_MASK PTJ_PTJ6_MASK

#define SPI_PORT PTM
#define SPI_PORT_DDR DDRM
#define SPI_SCK_MASK PTM_PTM5_MASK
#define SPI_MOSI_MASK PTM_PTM4_MASK
#define SPI_NSS_MASK PTM_PTM3_MASK
#define SPI_MISO_MASK PTM_PTM2_MASK
#define DAC_UPDATE_MASK 0x5000

#define DAC_UPLIMIT  127
#define DAC_LOWLIMIT 0


int last_floor = 0;
int floor = 0;
int DAC_Value;

// PID variables
/*
volatile int location;
volatile long error;
volatile long output;
volatile long old_error;
volatile long old_output;
volatile int setpoint;
*/


unsigned char data[DATA_LENGTH];

void SPIinit(void);
void DAC_START(void);
void DACwrite(char buf);
void putcSPI(char cx);
interrupt VECTOR_NUM( TIMER_CHNL_VECTOR( 2 ) ) void TC2_OC_handler( void );





//static volatile char tc5Count;
//static signed long volatile floor_request,output,last_output,error,height;


void main(void) {
  /* put your own code here */

	SET_TCNT_PRESCALE( TCNT_PRESCALE_8);
	TSCR1 = TSCR1_INIT;

	CANinit(PID);
	LCDinit();
	initDistance();
	LCDclear();
	SPIinit();

	while (!(CANCTL0&0x10));

	CANRFLG = 0xC3;
	CANRIER = 0x01;

    // Initalize the dac output with no offset so it does  not move	
	DAC_Value = 64;

	InitializeQueue(&rec);	// Create the Recieve Queue

	EnableInterrupts;


  for(;;) 
  {
 		if(IsQueueEmpty(rec) != 0) 
		{
			DeQueue(&rec);
			Parse_PID();
		}
		
		getDistance();
        DisableInterrupts;
        distance = ONTIME;
        //distance = convert(distance);
        EnableInterrupts;
        
        //LCDprintf("%d \r", distance);

        distance = convert(distance);
        //LCDclear();
        LCDprintf("%d \r", ONTIME);
        
        //DisableInterrupts;
        //location = distance;
       // EnableInterrupts;
        
        DACwrite(DAC_Value);
        
        // Alert that the elevator car has arrived
        
        if(distance < 10 && distance > 2)
             floor = F1;
        else if(distance < 63 && distance > 56)
            floor = F2;
            
        else if(distance < 123 && distance > 117)
            floor = F3;
        
        else
            floor = 0;
        
            
        
        // Say that the floor has changed
        if(floor != last_floor && floor != 0)
        {
            data[0] = PID;
            data[1] = REC_POINT;
            data[2] = floor;
            (void)CANSend(PID, 0x00, 0x00, DATA_LENGTH, data);
            last_floor = floor;   
        }
        
        if(dest_floor == F1)
        {
            while(distance > 10) {
              
              DisableInterrupts;
              distance = convert(ONTIME);
              EnableInterrupts;
              
              DACwrite(85);
            }
        } 
        else if(dest_floor == F3) 
        {
          while(distance < 116) {
              
              DisableInterrupts;
              distance = convert(ONTIME);
              EnableInterrupts;
              
              DACwrite(45);
          }
        } 
        else if(dest_floor == F2) 
        {
          if(distance < 59) {
            
            while(distance < 59) 
            {
               DisableInterrupts;
              distance = convert(ONTIME);
              EnableInterrupts;
              
              DACwrite(45);
            }
          }else if(distance > 61) 
          {
             while(distance > 61) 
            {
               DisableInterrupts;
              distance = convert(ONTIME);
              EnableInterrupts;
              
              DACwrite(85);
            }
          }
           
        }
            
        
        

             
		
  } /* loop forever */
  /* please make sure that you never leave main */
}

/**************putcSPI*********************************************************
Purpose: Write a character to the SPI port
Input:   char cx - character to write
******************************************************************************/
void putcSPI(char cx)
{
     while(!(SPISR & SPISR_SPTEF_MASK));
     
     SPIDR = cx;
     
     while(!(SPISR&SPISR_SPIF_MASK));
     
     (void)SPIDR;
}

/*******************SPInit()***************************************************
Purpose: Initalize the SPI PORT
*******************************************************************************/
void SPIinit(void)
{
  // Slave select
  SET_BITS(SPI_CS_PORT_DDR, SPI_CS1_MASK);
  // Init from class
  SPICR1 = SPICR1_INIT;
  // 1 MBPS
  SPIBR = SPI_DIV;
  // Initialize to on
  SET_BITS(SPI_CS_PORT, SPI_CS1_MASK);
}

/********************DAC_FINISH()**********************************************
Purpose: Turn the DAC OFF
         - Select Slave
         - Send upper byte
         - Send lower byte
         - Unselect slave
******************************************************************************/
void DAC_START(void)
{
   CLR_BITS(SPI_CS_PORT, SPI_CS1_MASK);
   putcSPI(ON >> 8);            
   putcSPI(ON & 0xFF);          
   SET_BITS(SPI_CS_PORT, SPI_CS1_MASK);
}

/********************DAC_FINISH()**********************************************
Purpose: Turn the DAC OFF
         - Select Slave
         - Send upper byte
         - Send lower byte
         - Unselect slave
******************************************************************************/
void DAC_FINISH(void)
{
  CLR_BITS(SPI_CS_PORT, SPI_CS1_MASK);
  putcSPI(OFF >> 8);
  putcSPI(OFF & 0xFF);   
  SET_BITS(SPI_CS_PORT, SPI_CS1_MASK);
}


/********************DAC_Write()**********************************************
Purpose: Write to the DAC through SPI
******************************************************************************/                                                                                                                             
void DACwrite(char buf)
{
    unsigned int write;
    write = (DAC_UPDATE_MASK | (buf << 4)); //create full command
    //select DAC as slave
    CLR_BITS(SPI_CS_PORT, SPI_CS1_MASK);
    putcSPI(write >> 8);
    putcSPI(write & 0xFF);
    //release slave
    SET_BITS(SPI_CS_PORT, SPI_CS1_MASK);
}    










