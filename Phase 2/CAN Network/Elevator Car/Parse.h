/*
Author: Josh Osborne
*/


#ifndef _PARSE_H
#define _PARSE_H

#define LED1_PORT PTS
#define LED1_DDR  DDRS
#define LED1_MASK 0b00000100
#define LED2_MASK 0b00001000

#define SWITCH_PORT PORTB
#define SWITCH_DDR  DDRB

#define SWITCH0  0b00000001 
#define SWITCH1  0b00000010 
#define SWITCH2  0b00000100 
#define SWITCH3  0b00001000 
#define SWITCH4  0b00010000 
#define SWITCH5  0b00100000 
#define SWITCH6  0b01000000 
#define SWITCH7  0b10000000 



#define LED_PORT PTT
#define LED_DDR  DDRT

#define LED0  0b00000001 
#define LED1  0b00000010 
#define LED2  0b00000100 
#define LED3  0b00001000 
#define LED4  0b00010000 
#define LED5  0b00100000 
#define LED6  0b01000000 
#define LED7  0b10000000

void Parse_Car(void);

#endif