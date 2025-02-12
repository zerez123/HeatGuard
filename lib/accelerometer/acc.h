#ifndef _ACC_H_
#define _ACC_H_

// ADC channels
#define XAXINPUT    A0 
#define YAXINPUT    A1  
#define ZAXINPUT    A2 

#define ACC_N_READS 5

typedef struct {
    int x;
    int y;
    int z;
    bool valid;
    bool changed;
  } accAxis_t;

// ACC functions
void accRead(void);
bool accGetCurPos(accAxis_t *d);
void accPrintData(unsigned int count);
void accPrintData1(accAxis_t *accAx);
bool accIsMove(accAxis_t *ref, int thr);

#endif //_ACC_H_
