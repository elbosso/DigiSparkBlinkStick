#include <DigiCDC.h>
//#include <stdio.h>
#include <Adafruit_NeoPixel.h>

#define INTERVAL 1000
#define PIXEL_PIN    0  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1  // Number of NeoPixels

short int Red = 0;
short int Green = 0;
short int Blue = 20;
byte theIndex=0;
byte mode=0;
char colorString[12];
byte ledState = LOW;             // ledState used to set the LED
long previousMillis = 0;        // will store last time LED was updated
byte blink=0;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {                
  // initialize the digital pin as an output.
  strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  strip.show();  // Initialize all pixels to 'off'
  SerialUSB.begin(); 
}

// the loop routine runs over and over again forever:
void loop() {
  
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

    strip.setPixelColor(0, ledState==HIGH?strip.Color(  Red,Green,Blue):strip.Color(0,0,0));         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match

  
   SerialUSB.refresh();               // keep usb alive // can alos use SerialUSB.refresh();
} 

