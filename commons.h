#ifndef COMMONS_H
#define COMMONS_H

#include <Arduino.h>
#include <DigiCDC.h>

extern short int Red ;
extern short int Green;
extern short int Blue;
extern byte theIndex;
extern byte mode;
extern char colorString[12];
extern byte ledState;             // ledState used to set the LED
extern long previousMillis;        // will store last time LED was updated
extern byte blink;

extern int blink_on;
extern int blink_off;

extern byte expectedstringlength;


void common_setup();
void command_interpreter();

#endif /* COMMONS_H */

