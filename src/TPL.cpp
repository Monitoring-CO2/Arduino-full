#include "TPL.h"

void TPL::setup() {
	pinMode(TPL_PIN_DONE, OUTPUT);
	pinMode(TPL_PIN_DELAY, INPUT_PULLDOWN);
	powerOff();
}

void TPL::powerOff() {
#ifdef SERIAL_DEBUG
	Serial.println("PowerOff TPL !");
#endif
	digitalWrite(TPL_PIN_DONE, HIGH);
	delay(200);
	digitalWrite(TPL_PIN_DONE, LOW);
}