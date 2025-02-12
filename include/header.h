#ifndef _HEADER_H_
#define _HEADER_H_

#include <Arduino.h>

#define DEBUGPRINT 
#define DBGLINELENGTH  50
#define SLEEPTIME     100
#define ACCMOVETHR      2

typedef enum {
  ST_START,
  ST_POWERON,
  ST_POWEROFF
} programState_t;

typedef enum {
    EV_NULL,
    EV_KEYPRESSED,
    EV_KEYRELEASED,
    EV_ACCMOVE,
    EV_LATST
} programEvent_t;


#ifdef DEBUGPRINT
    #define DBG_PRINT(fmt, ...) do { \
        char dbg_buf[DBGLINELENGTH]; \
        snprintf(dbg_buf, sizeof(dbg_buf), fmt, ##__VA_ARGS__); \
        Serial.println(dbg_buf); \
    } while (0)
#else
    #define DBG_PRINT(fmt, ...)  // Nothing
#endif

#endif //_HEADER_H_
