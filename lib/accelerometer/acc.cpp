#include "acc.h"
#include "..\..\include\header.h"

ACC::ACC() : accCurrPos({0, 0, 0, false, false}) {}

#if ACC_N_READS > 1
void ACC::makeAve()
{
    accCurrPos = {0, 0, 0, true, false};
    for(int i = 0; i < ACC_N_READS; ++i) {
        // printData(&accDebRead[i]);
        accCurrPos.x += accDebRead[i].x;
        accCurrPos.y += accDebRead[i].y;
        accCurrPos.z += accDebRead[i].z;
    }
    accCurrPos.x /= ACC_N_READS;
    accCurrPos.y /= ACC_N_READS;
    accCurrPos.z /= ACC_N_READS;

    // DBG_PRINT("Ave - ");
    // printData(&accCurrPos);
}
#endif

void ACC::read()
{
#if ACC_N_READS > 1
    if (accCnt < ACC_N_READS) {
        accDebRead[accCnt].x = analogRead(XAXINPUT);
        accDebRead[accCnt].y = analogRead(YAXINPUT);
        accDebRead[accCnt].z = analogRead(ZAXINPUT);
        accDebRead[accCnt].valid = true;
        accDebRead[accCnt].changed = false; // You might want to track changes here
        accCnt++;
    }
    else {
        makeAve();
        accCnt = 0;
    }
#else
    accCurrPos.x = analogRead(XAXINPUT);
    accCurrPos.y = analogRead(YAXINPUT);
    accCurrPos.z = analogRead(ZAXINPUT);
    accCurrPos.valid = true;
    accCurrPos.changed = false; // You might want to track changes here
#endif
}

bool ACC::getCurPos(accAxis_t *d)
{
    if (accCurrPos.valid) {
        *d = accCurrPos;
    }
    return accCurrPos.valid;
}

void ACC::printData(unsigned int count)
{
    accAxis_t accAx;

    if(getCurPos(&accAx)) {
        DBG_PRINT("%u Acc x %d y %d z %d", count, accAx.x, accAx.y, accAx.z);
    }
}

void ACC::printData(const accAxis_t *accAx) const
{
    DBG_PRINT("Acc x %d y %d z %d", accAx->x, accAx->y, accAx->z);
}

bool ACC::isMove(const accAxis_t *ref, int thr) const
{
    bool res = false;
    if(ref->valid && accCurrPos.valid) {
        if(abs(ref->x - accCurrPos.x) > thr) res = true;
        if(abs(ref->y - accCurrPos.y) > thr) res = true;
        if(abs(ref->z - accCurrPos.z) > thr) res = true;
    }
    if(res) {
        DBG_PRINT("Move detected, THR %d", thr);
        DBG_PRINT("Ref:");
        printData(ref);
        DBG_PRINT("======");
        printData(0);
    }
    return res;
}