
#include <DigiCDC.h>

#define RED 0
#define GREEN 1
#define BLUE 2


short int Red = 0;
short int Green = 0;
short int Blue = 20;
byte theIndex=0;
byte mode=0;
char colorString[12];
byte ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
byte blink=0;
int repetitions=-1;
int currentrepetition=-1;
int cycles=3;
int currentcycle=-1;

int blink_on=100;
int blink_off=400;
int cyclepause=1000;


byte cycle=0;
byte lastcycle=0;

short int spd[3];
short int off[3];



byte expectedstringlength;

void setup() {                
  // initialize the digital pin as an output.
    pinMode(0,OUTPUT);
    pinMode(1,OUTPUT);
    pinMode(2,OUTPUT);
  SerialUSB.begin(); 
  // assign random speed to each spot
  off[RED]=3;
  off[GREEN]=5;
  off[BLUE]=7;
}

void reset()
{
  Red = 0;
  Green = 0;
  Blue = 20;
  theIndex=0;
  mode=0;
  ledState = LOW;
  blink=0;
  blink_on=100;
  blink_off=400;
  repetitions=-1;
  currentrepetition=-1;
  cycles=3;
  currentcycle=-1;
  cyclepause=1000;
  cycle=0;
}

// the loop routine runs over and over again forever:
void loop() {
  setBlue();
  command_interpreter();
  if(mode==0)
  manage_blink();

    if((cycle==1)||(lastcycle==1))
  {
    if(cycle==0)
    {
      Red=0;
      Green=0;
      Blue=0;
    }
      else
      {
     unsigned long currentMillis = millis();
     if (currentMillis - previousMillis >= blink_on)
     { 
      Red=spd[RED];
      Green=spd[GREEN];
      Blue=spd[BLUE];
      for(byte i=0;i<3;++i)
      {
        spd[i]+=off[i];
        if(spd[i]>255)
         {
          spd[i]=255;
          off[i]=-off[i];
         }
         else if(spd[i]<0)
         {
          spd[i]=0;
          off[i]=-off[i];
         }
      }
      previousMillis=currentMillis;
    analogWrite(0,Red);
    analogWrite(1,Green);
     }
  }
  }
  else
  {

    analogWrite(0,ledState == LOW?0:Red);
    analogWrite(1,ledState == LOW?0:Green);
  }
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
short int managetimes()
{
  short int akku=-1;
      if(theIndex==expectedstringlength)
      {
        colorString[theIndex]=0;
        byte digit=0;
        akku=0;
        short int multiplier=1000;
        for(byte digit=0;digit<4;++digit)
        {
          short int number=(short int) colorString[digit] - 48;
          akku+=multiplier*number;
          multiplier/=10;
        }
        mode=0;
        theIndex=0;
      }
      return akku;
}
void command_interpreter()
{
    //turns led on and off based on sending 0 or 1 from serial terminal
  if (SerialUSB.available()) {
    char input = SerialUSB.read();
    switch (mode)
    {
    case 0:
    {
      if((input == '#')||(input == '+'))
      {
        cycle=0;
        mode=1;
      }
      else if((input == '*')||(input == '!'))
      {
        cycle=0;
        mode=2;
        expectedstringlength=11;
        theIndex=0;
      }
      else if(input == 'n')
      {
        cycle=0;
        mode=3;
        expectedstringlength=4;
        theIndex=0;
      }
      else if(input == 'f')
      {
        cycle=0;
        mode=4;
        expectedstringlength=4;
        theIndex=0;
      }
      else if(input == 'c')
      {
        cycle=1;
      }
      else if(input == 'r')
      {
        reset();
      }
      else if(input == 'p')
      {
        cycle=0;
        mode=5;
        expectedstringlength=4;
        theIndex=0;
      }
      else if(input == 'u')
      {
        cycle=0;
        repetitions=-1;
        currentrepetition=-1;
      }
      else if(input == 'P')
      {
        cycle=0;
        mode=6;
        expectedstringlength=4;
        theIndex=0;
      }
      else if(input == 'U')
      {
        cycle=0;
        cycles=-1;
        currentcycle=-1;
      }
      if((input == '+')||(input == '!'))
      {
        cycle=0;
        blink=1;
        currentrepetition=repetitions;
        currentcycle=cycles;
      }
      else if((input == '#')||(input == '*'))
      {
        cycle=0;
        blink=0;
      }
      break;
    }
    case 2:
    {
      colorString[theIndex]=input;
      ++theIndex;
      if(theIndex==expectedstringlength)
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
    case 3:
    {
      colorString[theIndex]=input;
      ++theIndex;
      short int akku=managetimes();
      if(akku>-1)
      {
        blink_on=akku;
        mode=0;
      }
      break;
    }
    case 4:
    {
      colorString[theIndex]=input;
      ++theIndex;
      short int akku=managetimes();
      if(akku>-1)
      {
        blink_off=akku;
        mode=0;
      }
      break;
    }
    case 5:
    {
      colorString[theIndex]=input;
      ++theIndex;
      short int akku=managetimes();
      if(akku>-1)
      {
        repetitions=akku;
        if((currentrepetition<0)||(currentrepetition>repetitions))
          currentrepetition=repetitions;
        mode=0;
      }
      break;
    }
    case 6:
    {
      colorString[theIndex]=input;
      ++theIndex;
      short int akku=managetimes();
      if(akku>-1)
      {
        cycles=akku;
        if((currentcycle<0)||(currentcycle>cycles))
          currentcycle=cycles;
        mode=0;
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

}
void manage_blink()
{
  if(blink==1)
  {
    unsigned long currentMillis = millis();
    //if(currentrepetition!=0)
    {
      if(ledState==HIGH)
      {
        if (currentMillis - previousMillis >= blink_on) 
        {
          previousMillis = currentMillis;
            ledState =LOW;
        }
      }
      else
      {
        if (currentMillis - previousMillis >= blink_off) 
        {
          if(currentrepetition!=0)
          {
            previousMillis = currentMillis;
            ledState =HIGH;
          }
          else
            ledState =LOW;
          if(currentrepetition>0)
            --currentrepetition;
        }
      }
    }
    if(currentrepetition==0)
//    else
    {
      if(ledState==HIGH)
      {
        if (currentMillis - previousMillis >= blink_off) 
        {
          previousMillis = currentMillis;
            ledState =LOW;
        }
      }
      else
      {
        if(currentcycle!=0)
        {
          if (currentMillis - previousMillis >= cyclepause) 
          {
            previousMillis = currentMillis;
              if(currentcycle>0)
              --currentcycle;
              if(currentcycle!=0)
              currentrepetition=repetitions;
          }
        }
      }
    }
  }
  else
    ledState=HIGH;
}

