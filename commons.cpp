#include "commons.h"

void common_setup() {                
  SerialUSB.begin(); 
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
        mode=1;
      }
      else if((input == '*')||(input == '!'))
      {
        mode=2;
        expectedstringlength=11;
        theIndex=0;
      }
      else if(input == 'n')
      {
        mode=3;
        expectedstringlength=4;
        theIndex=0;
      }
      else if(input == 'f')
      {
        mode=4;
        expectedstringlength=4;
        theIndex=0;
      }
      if((input == '+')||(input == '!'))
        blink=1;
      else if((input == '#')||(input == '*'))
        blink=0;
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
      if(theIndex==expectedstringlength)
      {
        colorString[theIndex]=0;
        byte digit=0;
        short int akku=0;
        short int multiplier=1000;
        for(byte digit=0;digit<4;++digit)
        {
          short int number=(short int) colorString[digit] - 48;
          akku+=multiplier*number;
          multiplier/=10;
        }
        blink_on=akku;
        mode=0;
        theIndex=0;
      }
      break;
    }
    case 4:
    {
      colorString[theIndex]=input;
      ++theIndex;
      if(theIndex==expectedstringlength)
      {
        colorString[theIndex]=0;
        byte digit=0;
        short int akku=0;
        short int multiplier=1000;
        for(byte digit=0;digit<4;++digit)
        {
          short int number=(short int) colorString[digit] - 48;
          akku+=multiplier*number;
          multiplier/=10;
        }
        blink_off=akku;
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

}

