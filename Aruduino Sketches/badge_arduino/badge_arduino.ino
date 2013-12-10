
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

#define NUM_IMAGES 3
#define LOOP_DURATION 12000

#define IMAGE_00_IMAGE "image_00.xbm"
#define IMAGE_00_BITS image_00_bits

#define IMAGE_01_IMAGE "image_01.xbm"
#define IMAGE_01_BITS image_01_bits

#define IMAGE_02_IMAGE "image_02.xbm"
#define IMAGE_02_BITS image_02_bits

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



// Arduino IO layout
const int Pin_PANEL_ON = 2;
const int Pin_BORDER = 3;
const int Pin_DISCHARGE = 4;
const int Pin_PWM = 5;
const int Pin_RESET = 6;
const int Pin_BUSY = 7;
const int Pin_EPD_CS = 8;

const int Pin_BUTTON = 9;

// pre-processor convert to string
#define MAKE_STRING1(X) #X
#define MAKE_STRING(X) MAKE_STRING1(X)


// define the E-Ink display
EPD_Class EPD(EPD_SIZE, Pin_PANEL_ON, Pin_BORDER, Pin_DISCHARGE, Pin_PWM, Pin_RESET, Pin_BUSY, Pin_EPD_CS, SPI);

static unsigned long t = 0;
static unsigned long elapsedTime = 0;
static int state = 0;
static int button_value = HIGH;

// I/O setup
void setup() {
  pinMode(Pin_PWM, OUTPUT);
  pinMode(Pin_BUSY, INPUT);
  pinMode(Pin_RESET, OUTPUT);
  pinMode(Pin_PANEL_ON, OUTPUT);
  pinMode(Pin_DISCHARGE, OUTPUT);
  pinMode(Pin_BORDER, OUTPUT);
  pinMode(Pin_EPD_CS, OUTPUT);
  pinMode(Pin_BUTTON, INPUT_PULLUP);

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

  button_value = digitalRead(Pin_BUTTON);
  elapsedTime = 0;
  t = millis();
}

// main loop
void loop() {          
  int _state = state;

  unsigned long _t = millis();
  elapsedTime += (_t - t);
  t = _t;

  int _button_value = digitalRead(Pin_BUTTON);
  if (_button_value != button_value) {
    if (_button_value == LOW) {
      _state = state + 1;
      if (_state >= NUM_IMAGES+1) {
        _state = 1;
      }
      elapsedTime = 0;
    }
    button_value = _button_value;
  }

  if (elapsedTime >= LOOP_DURATION) {
    _state = state + 1;
    if (_state >= NUM_IMAGES+1) {
      _state = 1;
    }
    elapsedTime = 0;
  }


  if (_state != state) {
    state = _state;

    EPD.begin(); // power up the EPD panel
    switch(state) {
    default:
    case 0:         // clear the screen
      EPD.image(IMAGE_00_BITS);
      break;
    case 1:         // clear -> text
      EPD.image(IMAGE_00_BITS, IMAGE_01_BITS);
      break;
    case 2:
      EPD.image(IMAGE_01_BITS, IMAGE_02_BITS);
      break;
    case 3:
      EPD.image(IMAGE_02_BITS, IMAGE_00_BITS);
      break;
    }
    EPD.end();   // power down the EPD panel
    delay(100);
  }
}






