/*
Purpose: Functions for the controller parse funcitons
 Author: Josh Osborne
   Date: June 6
Version: 1
*/


#ifndef _PARSE_H
#define _PARSE_H

void Floor(unsigned int floor, unsigned int sub_comp, unsigned int instruction);
void Parse_Data(void);
void Car(unsigned int sub_comp, unsigned int instruction);
void Feedback(unsigned int sub_comp, unsigned int instruction);

extern volatile unsigned int rxdata[DATA_LENGTH];

#endif