/*
 * Copyright (C) 2025 Erez Zuler
 *
 * This file is part of HeatGuard.
 *
 * HeatGuard is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * HeatGuard is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with HeatGuard. If not, see <https://www.gnu.org/licenses/>.
 */

 /*
 * Since it is designed for a product powered by the electrical grid, 
 * the code has been written without attention to code efficiency or 
 * current consumption.
 */


#include <avr/wdt.h>
#include "header.h"
#include "acc.h"
#include "deviceio.h"



// Globals
programState_t programState;
pbState_t      pushButState;
accAxis_t      accOnVal;

// put function declarations here:
void stateMachine(programEvent_t ev);

// Initializations
void setup() {
    Serial.begin(115200);      // Start serial communication for debugging
    analogReference(DEFAULT);  // Use default 5V reference (change to INTERNAL if needed)
    // Setup the GPIO
    pinMode(RLEDP, OUTPUT);
    pinMode(GLEDP, OUTPUT);
    digitalWrite(RELAYP, LOADOFF);
    pinMode(RELAYP, OUTPUT);
    pinMode(PBUTP, INPUT_PULLUP); 
    dioLedGreen(LEDON);
    dioLenRed(LEDON);
    programState = ST_START;
    stateMachine (EV_NULL);
    DBG_PRINT("Program start\n");
}


void stateMachine(programEvent_t ev)
{
  DBG_PRINT("State %d Event %d", programState, ev);
  switch(programState) {
    case ST_START:
      if (ev == EV_KEYPRESSED) {
        if (accGetCurPos(&accOnVal) == true) {
          programState = ST_POWERON;
          dioLedGreen(LEDON);
          dioLenRed(LEDOFF);
          digitalWrite(RELAYP, LOADON);
          DBG_PRINT("%s Locked", __func__);
          accPrintData1(&accOnVal);
        }
      }
    break;
    case ST_POWERON:
      if ((ev == EV_KEYPRESSED) || (ev == EV_ACCMOVE)) {
        programState = ST_POWEROFF;
        dioLedGreen(LEDOFF);
        dioLenRed(LEDON);
        digitalWrite(RELAYP, LOADOFF);
      }
    break;
    case ST_POWEROFF:
      if (ev == EV_KEYPRESSED) {
        programState = ST_POWERON;
        dioLedGreen(LEDON);
        dioLenRed(LEDOFF);
        digitalWrite(RELAYP, LOADON);
      }
    break;
  }
} 

// Periodics
void per100ms(unsigned int count)
{
  accRead();
  dioPbRead(count);
}

void per500ms(unsigned int count)
{

}

void per1000ms(unsigned int count)
{
  pbState_t pbs;
  bool      pbPressed = false;
  bool      pbReleased = false;
  bool      accChange = false;



// Handle push but change
  if (dioPbGetStat(&pbs) == true) {
    if(pushButState.valid == true) {
        if (pushButState.st == pbs.st) {
          pbPressed = false;
          pbReleased = false;
        }
        else {
          if(pbs.st == PUSHED) pbPressed = true;
          else                 pbReleased = true;
        }
      }
    pushButState = pbs;
  }

  accChange = accIsMove(&accOnVal, ACCMOVETHR);
  if (pbPressed == true) {
    stateMachine(EV_KEYPRESSED);
    pbPressed = false;
  }
  if (pbReleased == true) {
    stateMachine(EV_KEYRELEASED);
    pbReleased = false;
  }
  if(accChange == true) {
    stateMachine(EV_ACCMOVE);
    accChange = false;
  }

}

void per5000ms(unsigned int count)
{
  // if(count == 50) {
  //   stateMachine(EV_KEYPRESSED);
  // }
 
  accPrintData(count);

}

// Main loop
void loop() {
  static unsigned int cnt = 0;

  if (cnt % (100/SLEEPTIME) == 0) per100ms(cnt);
  if (cnt % (500/SLEEPTIME) == 0) per500ms(cnt);
  if (cnt % (1000/SLEEPTIME) == 0) per1000ms(cnt);
  if (cnt % (5000/SLEEPTIME) == 0) per5000ms(cnt);
  ++cnt;

  delay(SLEEPTIME);
}

