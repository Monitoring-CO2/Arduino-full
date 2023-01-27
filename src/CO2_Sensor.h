#ifndef MONITOR_CO2_SENSOR_CO2_SENSOR_H
#define MONITOR_CO2_SENSOR_CO2_SENSOR_H

#include <SparkFun_SCD30_Arduino_Library.h>
#include <Wire.h>
#include "TPL.h"
#include "CONFIG.h"

class CO2_Sensor {
public:
	void setup();
	bool getMeasure();

	float getTemperature() const;
	float getHumidity() const;
	int getCO2() const;
private:
	SCD30 airSensor;
	float temperature = 0;
	float humidity = 0;
	int co2 = 0;
};


#endif //MONITOR_CO2_SENSOR_CO2_SENSOR_H
