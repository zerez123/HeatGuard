#pragma once

#include <Arduino.h>

#define DEBUGPRINT
#define DBGLINELENGTH  50
#define SLEEPTIME      100
#define ACCMOVETHR     10

enum class programState_t {
    ST_START,
    ST_POWERON,
    ST_POWEROFF
};

enum class programEvent_t {
    EV_NULL,
    EV_KEYPRESSED,
    EV_KEYRELEASED,
    EV_ACCMOVE,
    EV_LAST
};

#ifdef DEBUGPRINT
    #define DBG_PRINT(fmt, ...) do { \
        char dbg_buf[DBGLINELENGTH]; \
        snprintf(dbg_buf, sizeof(dbg_buf), fmt, ##__VA_ARGS__); \
        Serial.println(dbg_buf); \
    } while (0)
#else
    #define DBG_PRINT(fmt, ...)  // Nothing
#endif
