/*
Purpose: Functions for the controller parse funcitons
 Author: Josh Osborne
   Date: June 6
Version: 1
*/


#ifndef _PARSE_H
#define _PARSE_H

void Floor(unsigned char floor, unsigned char sub_comp, unsigned char instruction);
void Parse_Data(void);
void Serial_Parse(void);
void Car(unsigned char sub_comp, unsigned char instruction);
void Feedback(unsigned char sub_comp, unsigned char instruction);

#endif