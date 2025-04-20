
#include "deviceio.h"
#include "..\..\include\header.h"
DeviceIO::DeviceIO() : pushButDebState({pushButState_t::RELAESED, false, false}) {
    pinMode(GLEDP, OUTPUT);
    pinMode(RLEDP, OUTPUT);
    pinMode(PBUTP, INPUT_PULLUP); // Assuming a pull-up resistor for the button
    pinMode(RELAYP, OUTPUT);
}

void DeviceIO::ledGreen(int s)
{
    digitalWrite(GLEDP, s);
}

void DeviceIO::ledRed(int s)
{
    digitalWrite(RLEDP, s);
}

void DeviceIO::Relay(int s)
{
    digitalWrite(RELAYP, s);
}

bool DeviceIO::pbGetStat(pbState_t *pbs)
{
    if (pushButDebState.valid) {
        *pbs = pushButDebState;
    }
    return pushButDebState.valid;
}

void DeviceIO::pbRead(unsigned int s)
{
    int pbStat = digitalRead(PBUTP);

    if (pbStat == pbLastStat) {
        ++pbDebCntr;
    }
    else {
        pbDebCntr = 0;
        pbLastStat = pbStat;
    }

    if (pbDebCntr >= 5) {
        if (PBPRESS == pbStat) {
            pushButDebState.st = pushButState_t::PUSHED;
        }
        else {
            pushButDebState.st = pushButState_t::RELAESED;
        }
        pushButDebState.valid = true;
        pushButDebState.changed = (pushButDebState.st != (pbStat == PBPRESS ? pushButState_t::PUSHED : pushButState_t::RELAESED));
        pbDebCntr = 0; // Reset counter after valid state
    }
}