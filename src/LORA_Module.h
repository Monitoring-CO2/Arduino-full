#ifndef MONITORING_CO2_LORA_LORA_MODULE_H
#define MONITORING_CO2_LORA_LORA_MODULE_H

#include <MKRWAN.h>
#include <ArduinoLowPower.h>
#include <RTCZero.h>
#include "EINK_Screen.h"
#include "DataStore.h"
#include "CONFIG.h"

class LORA_Module {
public:
	bool setup(RTCZero *rtcZero);
	explicit LORA_Module(EINK_Screen *einkScreen);
	void sendDataStore(LinkedList<CO2_data_t*> *dataList);
private:
	LoRaModem modem;
	void pollTime(RTCZero *rtcZero, unsigned int joinEpoch);
	EINK_Screen *einkScreen;
};


#endif //MONITORING_CO2_LORA_LORA_MODULE_H
