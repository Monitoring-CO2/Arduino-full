#include "CO2_Sensor.h"

void CO2_Sensor::setup() {
	Wire.begin();
}

bool CO2_Sensor::getMeasure() {
#ifdef SERIAL_DEBUG
	Serial.print("Starting airSensor...");
#endif
	while(!airSensor.begin(Wire, false)) {
		delay(500);
	}
	airSensor.setMeasurementInterval(2);
#ifdef SERIAL_DEBUG
	Serial.print("OK\nWaiting 6s for CO2 sensor...");
#endif
	LOWPOWER_SLEEP(6000);
#ifdef SERIAL_DEBUG
	Serial.print("OK\nStarting airSensor...");
#endif
	while(!airSensor.begin(Wire, false)) {
		delay(500);
	}
#ifdef SERIAL_DEBUG
	Serial.println("OK");
#endif

	if (airSensor.dataAvailable())
	{
		co2 = airSensor.getCO2();
		temperature = airSensor.getTemperature();
		humidity = airSensor.getHumidity();
#ifdef SERIAL_DEBUG
		Serial.print("co2(ppm):");
		Serial.print(co2);

		Serial.print(" temp(C):");
		Serial.print(temperature, 1);

		Serial.print(" humidity(%):");
		Serial.print(humidity, 1);

		Serial.println();
#endif
		TPL::powerOff();
		return true;
	}

	TPL::powerOff();
	return false;
}

float CO2_Sensor::getTemperature() const {
	return temperature;
}

float CO2_Sensor::getHumidity() const {
	return humidity;
}

int CO2_Sensor::getCO2() const {
	return co2;
}