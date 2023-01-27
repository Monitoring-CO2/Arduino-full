#ifndef MONITOR_CO2_SENSOR_TPL_H
#define MONITOR_CO2_SENSOR_TPL_H

#include <ArduinoLowPower.h>
#include <Arduino.h>
#include "CONFIG.h"

#define TPL_PIN_DONE 6
#define TPL_PIN_DELAY 7

class TPL {
public:
	static void setup();
	static void powerOff();
	static void tpl_interrupt();
};

#endif //MONITOR_CO2_SENSOR_TPL_H
