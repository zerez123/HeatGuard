
#pragma once

// Arduino Core Header
#include <Arduino.h>

// Other ports
#define RLEDP  3
#define GLEDP  4
#define RELAYP 5
#define PBUTP  2

#define LEDON  HIGH
#define LEDOFF LOW
#define LOADON HIGH
#define LOADOFF LOW
#define PBPRESS LOW // Push button press

enum class pushButState_t {
    PUSHED,
    RELAESED
};

struct pbState_t {
    pushButState_t st;
    bool valid;
    bool changed;
};

class DeviceIO {
private:
    pbState_t pushButDebState;
    unsigned int pbDebCntr = 0;
    int pbLastStat = HIGH; // Initialize to released state (assuming pull-up)

public:
    DeviceIO();
    void ledGreen(int s);
    void ledRed(int s);
    void Relay(int s);
    bool pbGetStat(pbState_t *pbs);
    void pbRead(unsigned int s);
};
