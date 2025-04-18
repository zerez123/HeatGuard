#include "..\..\include\header.h"
#include "acc.h"

static accAxis_t accDebRead[ACC_N_READS];
static accAxis_t accCurrPos;
static int accCnt = 0;


static void accMakeAve(void)
{
    accCurrPos = {0};
    for(int i = 0; i < ACC_N_READS; ++i) {
        // accPrintData1(&accDebRead[i]);
        accCurrPos.x += accDebRead[i].x;
        accCurrPos.y += accDebRead[i].y;
        accCurrPos.z += accDebRead[i].z;
    }
    accCurrPos.x /= ACC_N_READS;
    accCurrPos.y /= ACC_N_READS;
    accCurrPos.z /= ACC_N_READS;
    accCurrPos.valid = true;
    
    // DBG_PRINT("Ave - ");
    // accPrintData1(&accCurrPos);
}
 
void accRead(void)
{
    if (accCnt < ACC_N_READS) {
        accDebRead[accCnt].x = analogRead(XAXINPUT);
        accDebRead[accCnt].y = analogRead(YAXINPUT);
        accDebRead[accCnt].z = analogRead(ZAXINPUT); 
        accDebRead[accCnt].valid = true;
        accCnt++;
    }
    else {
        accMakeAve();
        accCnt = 0;
    }
}

bool accGetCurPos(accAxis_t *d)
{
    if (accCurrPos.valid == true)
        *d = accCurrPos;
    return accCurrPos.valid;

}

void accPrintData(unsigned int count)
{
    accAxis_t accAx;

    if(accGetCurPos(&accAx) == true) {
        DBG_PRINT("%u Acc x %d y %d z %d",count, accAx.x, accAx.y, accAx.z);
    }
}

void accPrintData1(accAxis_t *accAx)
{
    DBG_PRINT("Acc x %d y %d z %d", accAx->x, accAx->y, accAx->z);
}

bool accIsMove(accAxis_t *ref, int thr)
{
    bool res = false;
    if((ref->valid == true) && (accCurrPos.valid == true)) {
        if(abs(ref->x - accCurrPos.x) > thr) res |= true;
        if(abs(ref->y - accCurrPos.y) > thr) res |= true;
        if(abs(ref->z - accCurrPos.z) > thr) res |= true;
    }
    if(res) {
        DBG_PRINT("Move detected, THR %d", thr);
        accPrintData1(ref);
        accPrintData(0);


    }
    return res;
}