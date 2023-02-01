#include "CONFIG_PRIVATE.h"

//#define SERIAL_DEBUG

#ifndef SERIAL_DEBUG
#define LOWPOWER_SLEEP(ms) LowPower.deepSleep(ms)
#else
#define LOWPOWER_SLEEP(ms) delay(ms)
#endif

#define SAMPLES_PER_UPLOAD 6