#include "CONFIG_PRIVATE.h"

//#define SERIAL_DEBUG

#ifndef SERIAL_DEBUG
#define LOWPOWER_SLEEP(ms) LowPower.deepSleep(ms)
#define SAMPLES_PER_UPLOAD 6
#else
#define LOWPOWER_SLEEP(ms) delay(ms)
#define SAMPLES_PER_UPLOAD 1
#endif