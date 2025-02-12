#ifndef _DEVICEIO_H_
#define _DEVICEIO_H_

// Other ports
#define RLEDP   3
#define GLEDP   4
#define RELAYP  5
#define PBUTP   2

#define LEDON   HIGH
#define LEDOFF  LOW
#define LOADON  HIGH
#define LOADOFF LOW
#define PBPRESS LOW  //Push button press

typedef enum {
    PUSHED,
    RELAESED
} pushButState_t;

typedef struct {
   pushButState_t st;
   bool valid;
   bool changed;
} pbState_t;

void dioLedGreen(int s);
void dioLenRed(int s);
bool dioPbGetStat(pbState_t *pbs );
void dioPbRead (unsigned int s);


#endif //_DEVICEIO_H_