#ifndef MONITORING_CO2_FULL_MOTION_H
#define MONITORING_CO2_FULL_MOTION_H

#include <Arduino.h>
#include "CONFIG.h"

#define PIN_MOTION PIN_A6

class Motion {

public:

	Motion();
	~Motion();
	void captureData();
	double getProbaOccupe();

private:
	float *motionRawData;
	int motionRawDataCount = 0;
	void computeMotionProba();

	double probaOccupe = 0;
	int nbCompteProba = 0;
};


#endif //MONITORING_CO2_FULL_MOTION_H
