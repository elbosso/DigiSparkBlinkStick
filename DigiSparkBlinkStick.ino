#include <DigiCDC.h>
//#include <stdio.h>

short int Red = 0;
short int Green = 0;
short int Blue = 20;
byte theIndex=0;
byte mode=0;
char colorString[12];
byte ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
byte blink=0;

#define INTERVAL 1000

void setup() {                
  // initialize the digital pin as an output.
    pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
    pinMode(2,OUTPUT);
  SerialUSB.begin(); 
}

// the loop routine runs over and over again forever:
void loop() {
  setBlue();
  
  //turns led on and off based on sending 0 or 1 from serial terminal
  if (SerialUSB.available()) {
    char input = SerialUSB.read();
    switch (mode)
    {
    case 0:
    {
      blink=0;
      if((input == '#')||(input == '+'))
      {
        mode=1;
      }
      else if((input == '*')||(input == '!'))
      {
        mode=2;
        theIndex=0;
      }
      if((input == '+')||(input == '!'))
        blink=1;
      break;
    }
    case 2:
    {
      colorString[theIndex]=input;
      ++theIndex;
      if(theIndex==11)
      {
        colorString[theIndex]=0;
        byte color=0;
        byte digit=0;
        short int akku=0;
        short int multiplier=100;
        for(byte color=0;color<3;++color)
        {
          akku=0;
          multiplier=100;
          for(byte digit=0;digit<3;++digit)
          {
            short int number=(short int) colorString[color*4+digit] - 48;
            akku+=multiplier*number;
            multiplier/=10;
          }
          if(color==0)
            Red=akku;
          else if(color==1)
            Green=akku;
          else
            Blue=akku;
        }
//        sscanf(colorString,"%hd,%hd,%hd",&Red,&Green,&Blue);
        mode=0;
        theIndex=0;
      }
      break;
    }
    default:
    {
      int x = (int) input - 48;
      Red=0;
      Green=0;
      Blue=0;
      if(x&0x1)
        Red=255;
      if(x&0x2)
        Green=255;
      if(x&0x4)
        Blue=255;
      mode=0;
    }
    }
  }
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= INTERVAL) 
    {
      previousMillis = currentMillis;
      if(blink==1)
        ledState =ledState == LOW?HIGH:LOW;
      else
        ledState=HIGH;
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

