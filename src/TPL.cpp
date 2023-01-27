#include "TPL.h"

void TPL::setup() {
	pinMode(TPL_PIN_DONE, OUTPUT);
	pinMode(TPL_PIN_DELAY, INPUT_PULLDOWN);
	LowPower.attachInterruptWakeup(digitalPinToInterrupt(TPL_PIN_DELAY), tpl_interrupt, RISING);
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

void TPL::tpl_interrupt() {

}
