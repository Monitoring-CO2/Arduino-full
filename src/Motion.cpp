#include "Motion.h"
#include "Monitoring_CO2_Mouvement_inferencing.h"

Motion::Motion() {
	motionRawData = new float[EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE];
}

Motion::~Motion() {
	delete[] motionRawData;
}

void Motion::captureData() {
	pinMode(PIN_MOTION, INPUT);
	motionRawData[motionRawDataCount] = digitalRead(PIN_MOTION);
	motionRawDataCount ++;

	if(motionRawDataCount >= EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE){
		computeMotionProba();
		motionRawDataCount = 0;
	}
}

void Motion::computeMotionProba() {
	signal_t signal;
	int err = numpy::signal_from_buffer(motionRawData, EI_CLASSIFIER_DSP_INPUT_FRAME_SIZE, &signal);
	if (err != 0) {
#ifdef SERIAL_DEBUG
		Serial.print("Failed to create signal from buffer : ");
		Serial.println(err);
#endif
		return;
	}

	ei_impulse_result_t result = { 0 };
	err = run_classifier(&signal, &result, false);

	if (err != 0) {
#ifdef SERIAL_DEBUG
		Serial.print("Failed to run classifier : ");
		Serial.println(err);
#endif
		return;
	}

	//result.classification[0]; //Non occupé
	//result.classification[1]; //Occupé
	nbCompteProba++;
	probaOccupe += result.classification[1].value;

#ifdef SERIAL_DEBUG
	Serial.print("Computed motion probability : ");
	Serial.println(result.classification[1].value);
#endif
}

double Motion::getProbaOccupe() {
	double ret = probaOccupe/nbCompteProba;

	nbCompteProba = 0;
	probaOccupe = 0;

#ifdef SERIAL_DEBUG
	Serial.print("Retrieve motion probability : ");
	Serial.println(ret);
#endif

	return ret;
}
