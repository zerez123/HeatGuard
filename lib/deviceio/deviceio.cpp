#include <Arduino.h>
#include "..\..\include\header.h"
#include "deviceio.h"

static pbState_t  pushButDebState;

void dioLedGreen(int s)
{
  digitalWrite(GLEDP, s);
}

void dioLenRed(int s)
{
  digitalWrite(GLEDP, s);
}

bool dioPbGetStat(pbState_t *pbs )
{
    if(pushButDebState.valid == true) {
        *pbs = pushButDebState;
    }
    return pushButDebState.valid;
}

void dioPbRead (unsigned int s)
{
  static int pbDebCntr = 0;
  static int pbLastStat;
  int pbStat = digitalRead(PBUTP);

  if (pbStat == pbLastStat) {
    ++pbDebCntr;
  }
  else {
    pbDebCntr = 0;
    pbLastStat = pbStat;
  }
  if (pbDebCntr == 5) {
    if (PBPRESS == pbStat) pushButDebState.st = PUSHED;
    else                   pushButDebState.st = RELAESED;
    pushButDebState.valid = true;
    pushButDebState.changed = false;
  }
} 