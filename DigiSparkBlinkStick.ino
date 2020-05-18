
#include "commons.h"

short int Red = 0;
short int Green = 0;
short int Blue = 20;
byte theIndex=0;
byte mode=0;
char colorString[12];
byte ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
byte blink=0;

int blink_on=100;
int blink_off=1000;

byte expectedstringlength;

void setup() {                
  // initialize the digital pin as an output.
    pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
    pinMode(2,OUTPUT);
  common_setup();
}

// the loop routine runs over and over again forever:
void loop() {
  setBlue();
  command_interpreter();
    unsigned long currentMillis = millis();
    if(ledState==HIGH)
    {
      if (currentMillis - previousMillis >= blink_on) 
      {
        previousMillis = currentMillis;
        if(blink==1)
          ledState =ledState == LOW?HIGH:LOW;
        else
          ledState=HIGH;
      }
    }
    else
    {
      if (currentMillis - previousMillis >= blink_off) 
      {
        previousMillis = currentMillis;
        if(blink==1)
          ledState =ledState == LOW?HIGH:LOW;
        else
          ledState=HIGH;
      }
    }
        
    analogWrite(0,ledState == LOW?0:Red);
    analogWrite(1,ledState == LOW?0:Green);
     setBlue();

  
   SerialUSB.refresh();               // keep usb alive // can alos use SerialUSB.refresh();
}
void setBlue(){
    if((ledState == LOW)||(Blue == 0)){
      digitalWrite(2,LOW);
      return;
    }
    else if(Blue == 255){
      digitalWrite(2,HIGH);
      return;
    }
    // On period  
    for (int x=0;x<Blue;x++){
    digitalWrite(2,HIGH);
    } 
    
    // Off period
    
    for(int x=0;x<(255-Blue);x++){
    digitalWrite(2,LOW);
    }
  
} 

