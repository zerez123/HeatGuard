#pragma once

// Arduino Core Header
#include <Arduino.h>

// ADC channels
#define XAXINPUT A2
#define YAXINPUT A1
#define ZAXINPUT A0

#define ACC_N_READS 3

struct accAxis_t {
    int x;
    int y;
    int z;
    bool valid;
    bool changed;
};

class ACC {
private:
#if ACC_N_READS > 1
    accAxis_t accDebRead[ACC_N_READS];
    int accCnt = 0;
    void makeAve();
#endif
    accAxis_t accCurrPos;

public:
    ACC(); // Constructor
    void read();
    bool getCurPos(accAxis_t *d);
    void printData(unsigned int count);
    void printData(const accAxis_t *accAx) const;
    bool isMove(const accAxis_t *ref, int thr) const;
};
