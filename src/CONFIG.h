#include "CONFIG_PRIVATE.h"

#define SERIAL_DEBUG
#define LOWPOWER_SLEEP(ms) LowPower.deepSleep(ms)
//#define LOWPOWER_SLEEP(ms) delay(ms)