#ifndef MONITORING_CO2_EINK_SCREEN_H
#define MONITORING_CO2_EINK_SCREEN_H

#include <Arduino.h>
#include <Adafruit_ThinkInk.h>
#include <RTCZero.h>
#include <Timezone.h>
#include "EINK_Screen_bitmaps.h"

#define EPD_DC      4
#define EPD_CS      5
#define EPD_BUSY    1
#define SRAM_CS     3
#define EPD_RESET   2
#define EPD_ENA		0

static TimeChangeRule myDST = {"CET", Last, Sun, Mar, 2, +120};    //Daylight time = UTC + 2 hours
static TimeChangeRule mySTD = {"CET", Last, Sun, Oct, 2, +60};     //Standard time = UTC + 1 hours
static Timezone myTZ(myDST, mySTD);

class EINK_Screen {
private:
	ThinkInk_420_Tricolor_RW *display = new ThinkInk_420_Tricolor_RW(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);
	void powerOff();
	void powerOn();

	int batterie = 99;

	int minPPM = 400;
	int maxPPM = 2100;

	float temperature = 0.0f;
	int humidite = 0;
	int co2 = 0;

public:
	void setup();
	void drawLoraSetup(int phase);
	void drawMainFrame();
	void setSensorValues(float new_temperature, int new_humidite, int new_co2);
};


#endif //MONITORING_CO2_EINK_SCREEN_H
