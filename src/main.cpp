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

 #include <Arduino.h>
 #include "header.h"
 #include "acc.h"
 #include "deviceio.h"
 
 class HeatGuard {
 private:
     programState_t programState = programState_t::ST_START;
     pbState_t pushButState;
     accAxis_t accOnVal;
     ACC acc;
     DeviceIO dio;
 
     void enterOnState();
     void enterOffState();
     void stateMachine(programEvent_t ev);
 
 public:
     void setup();
     void loop();
     void per100ms(unsigned int count);
     void per500ms(unsigned int count);
     void per1000ms(unsigned int count);
     void per5000ms(unsigned int count);
 };
 
 HeatGuard heatGuard; // Create an instance of the HeatGuard class
 
 void HeatGuard::setup() {
     Serial.begin(115200);         // Start serial communication for debugging
     analogReference(DEFAULT);    // Use default 5V reference (change to INTERNAL if needed)
     // Setup the GPIO (moved to DeviceIO constructor)
 
     dio.ledGreen(LEDON);
     dio.ledRed(LEDON);
     stateMachine(programEvent_t::EV_NULL);
     DBG_PRINT("Program start\n");
 }
 
 void HeatGuard::enterOnState() {
     if (acc.getCurPos(&accOnVal)) {
         programState = programState_t::ST_POWERON;
         dio.ledGreen(LEDON);
         dio.ledRed(LEDOFF);
         dio.Relay(LOADON); 
         acc.printData(&accOnVal);
     }
 }
 
 void HeatGuard::enterOffState() {
     programState = programState_t::ST_POWEROFF;
     dio.ledGreen(LEDOFF);
     dio.ledRed(LEDON);
     dio.Relay(LOADOFF); 
 }
 
 void HeatGuard::stateMachine(programEvent_t ev) {
     DBG_PRINT("State %d Event %d", static_cast<int>(programState), static_cast<int>(ev));
     switch (programState) {
     case programState_t::ST_START:
         if (ev == programEvent_t::EV_KEYPRESSED) {
             enterOnState();
         }
         break;
     case programState_t::ST_POWERON:
         if ((ev == programEvent_t::EV_KEYPRESSED) || (ev == programEvent_t::EV_ACCMOVE)) {
             enterOffState();
         }
         break;
     case programState_t::ST_POWEROFF:
         if (ev == programEvent_t::EV_KEYPRESSED) {
             enterOnState();
         }
         break;
     }
 }
 
 void HeatGuard::per100ms(unsigned int count) {
     acc.read();
     dio.pbRead(count);
 }
 
 void HeatGuard::per500ms(unsigned int count) {
     pbState_t pbs;
     bool pbPressed = false;
     bool pbReleased = false;
     bool accChange = false;
 
     // Handle push but change
     if (dio.pbGetStat(&pbs)) {
         if (pushButState.valid) {
             if (pushButState.st == pbs.st) {
                 pbPressed = false;
                 pbReleased = false;
             } else {
                 if (pbs.st == pushButState_t::PUSHED) pbPressed = true;
                 else pbReleased = true;
             }
         }
         pushButState = pbs;
     }
 
     if (pbPressed) {
         stateMachine(programEvent_t::EV_KEYPRESSED);
         pbPressed = false;
     }
     if (pbReleased) {
         stateMachine(programEvent_t::EV_KEYRELEASED);
         pbReleased = false;
     }
     accChange = acc.isMove(&accOnVal, ACCMOVETHR);
     if (accChange) {
         stateMachine(programEvent_t::EV_ACCMOVE);
         accChange = false;
     }
 }
 
 void HeatGuard::per1000ms(unsigned int count) {
     // Add 1000ms periodic tasks here
 }
 
 void HeatGuard::per5000ms(unsigned int count) {
     acc.printData(count);
 }
 
 void HeatGuard::loop() {
     static unsigned int cnt = 0;
 
     if (cnt % (100 / SLEEPTIME) == 0) per100ms(cnt);
     if (cnt % (500 / SLEEPTIME) == 0) per500ms(cnt);
     if (cnt % (1000 / SLEEPTIME) == 0) per1000ms(cnt);
     if (cnt % (5000 / SLEEPTIME) == 0) per5000ms(cnt);
     ++cnt;
 
     delay(SLEEPTIME);
 }
 
 void setup() {
     heatGuard.setup();
 }
 
 void loop() {
     heatGuard.loop();
 }