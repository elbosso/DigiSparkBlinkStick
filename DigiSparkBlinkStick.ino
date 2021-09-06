#include <DigiCDC.h>
//#include <stdio.h>
//#include <Adafruit_NeoPixel.h>

//#define PIXEL_PIN    0  // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT 1  // Number of NeoPixels

#define RED 0
#define GREEN 1
#define BLUE 2

short int pixel[3];
short int colors[3][PIXEL_COUNT];

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
  for(int j=0;j<PIXEL_COUNT;++j)
  {
    pixel[j]=0;
    for(int i=0;i<PIXEL_COUNT;++i)
    {
      colors[j][i]=0;
    }
  }
  // initialize pseudo-random number generator with some random value
  //randomSeed(analogRead(RNDPIN));
  DDRB=0b00000001;
  //strip.begin(); // Initialize NeoPixel strip object (REQUIRED)
  //strip.show();  // Initialize all pixels to 'off'
  SerialUSB.begin(); 
  // assign random speed to each spot
  off[RED]=3;
  off[GREEN]=5;
  off[BLUE]=7;

}


void reset()
{
  for(int j=0;j<PIXEL_COUNT;++j)
  {
    pixel[j]=0;
    for(int i=0;i<PIXEL_COUNT;++i)
    {
      colors[j][i]=0;
    }
  }
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

  command_interpreter();
  if(mode==0)
    manage_blink();

  if((cycle==1)||(lastcycle==1))
  {
    if(cycle==0)
      send(0,0,0);
      else
      {
     unsigned long currentMillis = millis();
     if (currentMillis - previousMillis >= blink_on)
     { 
      send(spd[RED],spd[GREEN],spd[BLUE]);
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
     }
      }
    lastcycle=cycle;
  }
  else
  {
    short int c[3][PIXEL_COUNT];
    short int doit=0;
    for(int i=0;i<PIXEL_COUNT;++i)
    {
  
      byte a=((blink==0)&&(ledState==HIGH))||((blink==1)&&((ledState==HIGH)&&(i%2==0))||((ledState==LOW)&&(i%2==1)));
      for(int j=0;j<3;++j)
        c[j][i]=a?pixel[j]:0;
      if((c[RED][i]!=colors[RED][i])||((c[GREEN][i]!=colors[GREEN][i])||(c[BLUE][i]!=colors[BLUE][i])))
        doit=1;
    }
    if(doit==1)
    {
      for(int i=0;i<PIXEL_COUNT;++i)
      {
        send(c[RED][i],c[GREEN][i],c[BLUE][i]);
        for(int j=0;j<3;++j)
          colors[j][i]=c[j][i];
      }
    }
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
        ((1UL << x) & red) ? send1() : send0();
    }
    x = 8;
    while (x--) {
        ((1UL << x) & grn) ? send1() : send0();
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
      else if(input == 'r')
      {
        reset();
      }
      else if(input == 'c')
      {
        cycle=1;
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
            pixel[color]=akku;
          }
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
      for(int j=0;j<3;++j)
        pixel[j]=0;
      if(x&0x1)
        pixel[RED]=255;
      if(x&0x2)
        pixel[GREEN]=255;
      if(x&0x4)
        pixel[BLUE]=255;
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

