/*
Purpose: Header for the floor node parsing c file
 Author: Josh Osborne
   Date: June 6
Version: 1
*/

#ifndef _PARSE_H
#define _PARSE_H



#define SEG_PORT PORTA
#define SEG_DDR DDRA

#define LED_PORT PTS
#define LED_DDR  DDRS
#define LED1_MASK 0b00000100
#define LED2_MASK 0b00001000

#define SWITCH_PORT  PTJ
#define SWITCH_DDR   DDRJ
#define SWITCH1_MASK 0b10000000
#define SWITCH2_MASK 0b01000000

#define ID_DDR       DDRB
#define ID_MASK      0b11000000



// seven seg pattern defines

#define SEG_0  0b10000001   // 0
#define SEG_1  0b11110011   // 1
#define SEG_2  0b01001001   // 2
#define SEG_3  0b01100001   // 3
#define SEG_4  0b00110011   // 4
#define SEG_5  0b00100101   // 5
#define SEG_6  0b00000101   // 6
#define SEG_7  0b11110001   // 7
#define SEG_8  0b00000001   // 8
#define SEG_9  0b00100001   // 9
#define SEG_BLANK  0b11111111   // blank segment

extern int segval[];

extern int count;
extern int i;
extern int node_id;

extern int led;
extern int last;

void clear_seg(void);
void set_seg(unsigned int i);
void seg_init(void);
void Floor(unsigned int floor, unsigned int sub_comp, unsigned int instruction);
void Parse_Floor(void);

#define THE_FLOOR F1

#endif