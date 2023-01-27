#include <Arduino.h>
#include <ArduinoLowPower.h>
#include "CONFIG.h"
#include "EINK_Screen.h"
#include "CO2_Sensor.h"
#include "TPL.h"
#include "LORA_Module.h"
#include "DataStore.h"

EINK_Screen einkScreen;
CO2_Sensor co2Sensor;
LORA_Module loraModule(&einkScreen);
DataStore dataStore;
RTCZero rtcZero;

void update();

void setup() {
#ifdef SERIAL_DEBUG
	Serial.begin(115200);
	LOWPOWER_SLEEP(2000);
	Serial.println("Program started !");
#endif

	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, LOW);
	rtcZero.begin(true);
	einkScreen.setup();
	co2Sensor.setup();
	TPL::setup();
	loraModule.setup(&rtcZero);
	TPL::powerOff();
}

void loop() {
	LowPower.deepSleep();
	update();
}

void update(){
#ifdef SERIAL_DEBUG
	Serial.println("Requesting update");
#endif
	if(co2Sensor.getMeasure()){
		dataStore.addData(&rtcZero, co2Sensor.getTemperature(), (int)co2Sensor.getHumidity(), co2Sensor.getCO2(), 0);
		einkScreen.setSensorValues(co2Sensor.getTemperature(), (int)co2Sensor.getHumidity(), co2Sensor.getCO2());
		einkScreen.drawMainFrame();
#ifdef SERIAL_DEBUG
		Serial.println(String(dataStore.getDataList()->size())+" data stored");
#endif
		if(dataStore.getDataList()->size() >= 6){
			loraModule.sendDataStore(dataStore.getDataList());
		}
	}
	else{
#ifdef SERIAL_DEBUG
		Serial.println("Could not get measure from CO2 sensor !");
#endif
	}
}