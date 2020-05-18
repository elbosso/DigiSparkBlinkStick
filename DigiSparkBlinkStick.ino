#include <DigiCDC.h>
//#include <stdio.h>
//#include <Adafruit_NeoPixel.h>

//#define PIXEL_PIN    0  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1  // Number of NeoPixels

short int Red = 0;
short int Green = 0;
short int Blue = 20;
short int lRed = 0;
short int lGreen = 0;
short int lBlue = 0;

byte theIndex=0;
byte mode=0;
char colorString[12];
byte ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
byte blink=0;
int repetitions=-1;
int currentrepetition=-1;

int blink_on=100;
int blink_off=1000;

byte expectedstringlength;

// Declare our NeoPixel strip object:
//Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() { 
  DDRB=0b00000001;
  //strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  //strip.show();  // Initialize all pixels to 'off'
  SerialUSB.begin(); 
}

void reset()
{
  Red = 0;
  Green = 0;
  Blue = 20;
  lRed = 0;
  lGreen = 0;
  lBlue = 0;
  theIndex=0;
  mode=0;
  ledState = LOW;
  blink=0;
  blink_on=100;
  blink_off=1000;
  repetitions=-1;
  currentrepetition=-1;
}

// the loop routine runs over and over again forever:
void loop() {

  command_interpreter();
  if(mode==0)
  manage_blink();

//    strip.setPixelColor(0, ledState==HIGH?strip.Color(  Red,Green,Blue):strip.Color(0,0,0));         //  Set pixel's color (in RAM)
//    strip.show();     //  Update strip to match
short int r=ledState==HIGH?Red:0;
short int g=ledState==HIGH?Green:0;
short int b=ledState==HIGH?Blue:0;
if((r!=lRed)||((g!=lGreen)||(b!=lBlue)))
{
send(r,g,b);
lRed=r;
lGreen=g;
lBlue=b;
}
  
   SerialUSB.refresh();               // keep usb alive // can alos use SerialUSB.refresh();
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
//https://forum.arduino.cc/index.php?topic=531302.0
//http://shallowsky.com/blog/hardware/attiny85-c.html
////////////////////////////// WS2812B One Wire Timing ///////////////////////////////
//
//           _______________
//          |               |                        |
//  0 code: | <-- 350ns --> | <------ 900 ns ------> | <-- 1250 ns total (20~ @ 16mHz)
//          |               |________________________|
//
//
//           ________________________
//          |                        |               |
//  1 code: | <------ 900 ns ------> | <-- 350ns --> | <-- 1250 ns total (20~ @ 16mHz)
//          |                        |_______________|
//
//
//
//          |                                              |
//  RESET   | <------------- 50 us or more --------------> | <-- 50 us (800~ @ 16mHz)
//  Code:   |______________________________________________|
//
//
//////////////////////////////////////////////////////////////////////////////////////
// note: data order for ws281x is GRN, RED, BLU (MSB First)
//
//      | <------- GRN -------> | <------- RED -------> | <------- BLU -------> |
// BIT: |23 22 21 20 19 18 17 16|15 14 13 12 11 10  9  8| 7  6  5  4  3  2  1  0|
//////////////////////////////////////////////////////////////////////////////////////

void send0 (void) // send a "0" bit
{
    //sbi (LED_OUT, LED_BIT);
    //digitalWrite(LED_OUT, 1);
    PORTB=0b00000001;
    __builtin_avr_delay_cycles ((((F_CPU / 1e9) * 350) + 0.5) - 2);
    //cbi (LED_OUT, LED_BIT);
    //digitalWrite(LED_OUT, 0);
    PORTB=0b00000000;
    __builtin_avr_delay_cycles ((((F_CPU / 1e9) * 900) + 0.5) - 2);
}

void send1 (void) // send a "1" bit
{
    //sbi (LED_OUT, LED_BIT);
    //digitalWrite(LED_OUT, 1);
    PORTB=0b00000001;
    __builtin_avr_delay_cycles ((((F_CPU / 1e9) * 900) + 0.5) - 2);
    //cbi (LED_OUT, LED_BIT);
    //digitalWrite(LED_OUT, 0);
    PORTB=0b00000000;
    __builtin_avr_delay_cycles ((((F_CPU / 1e9) * 350) + 0.5) - 2);
}

void send (uint8_t red, uint8_t grn, uint8_t blu)
{
    // note: data order for ws281x is G,R,B
    uint8_t x;
    x = 8;
    while (x--) {
        ((1UL << x) & grn) ? send1() : send0();
    }
    x = 8;
    while (x--) {
        ((1UL << x) & red) ? send1() : send0();
    }
    x = 8;
    while (x--) {
        ((1UL << x) & blu) ? send1() : send0();
    }
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
      else if(input == 'r')
      {
        reset();
      }
      else if(input == 'p')
      {
        mode=5;
        expectedstringlength=4;
        theIndex=0;
      }
      else if(input == 'u')
      {
        repetitions=-1;
        currentrepetition=-1;
      }
      if((input == '+')||(input == '!'))
      {
        blink=1;
        currentrepetition=repetitions;
      }
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
      short int akku=managetimes();
      if(akku>-1)
      {
        blink_on=akku;
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
          previousMillis = currentMillis;
            ledState =currentrepetition!=0?HIGH:LOW;
          if(currentrepetition>0)
            --currentrepetition;
        }
      }
    }
/*    else
    {
      if(ledState==HIGH)
      {
        if (currentMillis - previousMillis >= blink_on) 
        {
          previousMillis = currentMillis;
            ledState =LOW;
        }
      }
    }
*/  }
  else
    ledState=HIGH;
}

