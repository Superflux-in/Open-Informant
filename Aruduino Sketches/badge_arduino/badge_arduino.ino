
// Copyright 2013 Pervasive Displays, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at:
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
// express or implied.  See the License for the specific language
// governing permissions and limitations under the License.


// This program is to illustrate the display operation as described in
// the datasheets.  The code is in a simple linear fashion and all the
// delays are set to maximum, but the SPI clock is set lower than its
// limit.  Therfore the display sequence will be much slower than
// normal and all of the individual display stages be clearly visible.


//
// Embedded Artists has modified Pervasive Display Inc's demo application 
// to run on the 2.7 inch E-paper Display module (EA-LCD-009 
//


#include <inttypes.h>
#include <ctype.h>

#include <SPI.h>
#include <Wire.h>
#include <EPD.h>

#define VERSION "1.0.0"

#define EPD_SIZE EPD_2_7

// configure images for display size
// change these to match display size above

#define IMAGE_00_IMAGE "image_00.xbm"
#define IMAGE_00_BITS image_00_bits

#define IMAGE_01_IMAGE "image_01.xbm"
#define IMAGE_01_BITS image_01_bits

#define IMAGE_02_IMAGE "image_02.xbm"
#define IMAGE_02_BITS image_02_bits

//#define IMAGE_03_IMAGE "image_03.xbm"
//#define IMAGE_03_BITS image_03_bits
//
//#define IMAGE_04_IMAGE "image_04.xbm"
//#define IMAGE_04_BITS image_04_bits
//
//#define IMAGE_05_IMAGE "image_05.xbm"
//#define IMAGE_05_BITS image_05_bits
//

// Add Images library to compiler path
#include <Images.h>  // this is just an empty file

// images
PROGMEM const
#define unsigned
#define char uint8_t
#include IMAGE_00_IMAGE
#undef char
#undef unsigned

PROGMEM const
#define unsigned
#define char uint8_t
#include IMAGE_01_IMAGE
#undef char
#undef unsigned

PROGMEM const
#define unsigned
#define char uint8_t
#include IMAGE_02_IMAGE
#undef char
#undef unsigned

//PROGMEM const
//#define unsigned
//#define char uint8_t
//#include IMAGE_03_IMAGE
//#undef char
//#undef unsigned
//
//PROGMEM const
//#define unsigned
//#define char uint8_t
//#include IMAGE_04_IMAGE
//#undef char
//#undef unsigned
//
//PROGMEM const
//#define unsigned
//#define char uint8_t
//#include IMAGE_05_IMAGE
//#undef char
//#undef unsigned
//


// Arduino IO layout
const int Pin_PANEL_ON = 2;
const int Pin_BORDER = 3;
const int Pin_DISCHARGE = 4;
const int Pin_PWM = 5;
const int Pin_RESET = 6;
const int Pin_BUSY = 7;
const int Pin_EPD_CS = 8;

// pre-processor convert to string
#define MAKE_STRING1(X) #X
#define MAKE_STRING(X) MAKE_STRING1(X)


// define the E-Ink display
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS, SPI);

// I/O setup
void setup() {
  pinMode(Pin_PWM, OUTPUT);
  pinMode(Pin_BUSY, INPUT);
  pinMode(Pin_RESET, OUTPUT);
  pinMode(Pin_PANEL_ON, OUTPUT);
  pinMode(Pin_DISCHARGE, OUTPUT);
  pinMode(Pin_BORDER, OUTPUT);
  pinMode(Pin_EPD_CS, OUTPUT);

  digitalWrite(Pin_PWM, LOW);
  digitalWrite(Pin_RESET, LOW);
  digitalWrite(Pin_PANEL_ON, LOW);
  digitalWrite(Pin_DISCHARGE, LOW);
  digitalWrite(Pin_BORDER, LOW);
  digitalWrite(Pin_EPD_CS, LOW);

  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setClockDivider(SPI_CLOCK_DIV4);

  Serial.begin(9600);
#if !defined(__MSP430_CPU__)
  // wait for USB CDC serial port to connect.  Arduino Leonardo only
  while (!Serial) {
  }
#endif
  Serial.println();
  Serial.println("Software version: " VERSION);
  Serial.println();
}


static int state = 0;


// main loop
void loop() {          
  EPD.begin(); // power up the EPD panel

  switch(state) {
  default:
  case 0:         // clear the screen
    EPD.clear();
    state = 1;
    break;

  case 1:         // clear -> text
    EPD.image(IMAGE_00_BITS);
    ++state;
    break;
  case 2:
    EPD.image(IMAGE_00_BITS, IMAGE_01_BITS);
    ++state;
    break;
  case 3:
    EPD.image(IMAGE_01_BITS, IMAGE_02_BITS);
    ++state;
    break;
  case 4:
    EPD.image(IMAGE_02_BITS, IMAGE_00_BITS);
    state = 2;
    break;
    //  case 4:
    //    EPD.image(IMAGE_01_BITS, IMAGE_02_BITS);
    //    ++state;
    //    break;
    //  case 5:
    //    EPD.image(IMAGE_02_BITS, IMAGE_03_BITS);
    //    ++state;
    //    break;
    //  case 6:
    //    EPD.image(IMAGE_03_BITS, IMAGE_04_BITS);
    //    ++state;
    //    break;
    //  case 7:
    //    EPD.image(IMAGE_04_BITS, IMAGE_05_BITS);
    //    ++state;
    //    break;
  }
  EPD.end();   // power down the EPD panel

  delay(5000);
}


