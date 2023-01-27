#include "LORA_Module.h"

LORA_Module::LORA_Module(EINK_Screen *einkScreen) {
	this->einkScreen = einkScreen;
}

bool LORA_Module::setup(RTCZero *rtcZero) {
	rtcZero->begin(true);
	einkScreen->drawLoraSetup(1);
#ifdef SERIAL_DEBUG
	Serial.print("Starting LoRa...");
#endif
	if(!modem.begin(EU868)){
		return false;
	}

#ifdef SERIAL_DEBUG
	Serial.print("OK !\nJoining...");
#endif
	modem.dataRate(5);
	modem.minPollInterval(2);
	modem.setPort(255);

	if(!modem.joinOTAA(LORA_APP_EUI, LORA_APP_KEY)){
		return false;
	}
	unsigned int joinEpoch = rtcZero->getEpoch();
#ifdef SERIAL_DEBUG
	Serial.println("OK !");
#endif

	einkScreen->drawLoraSetup(2);
	LOWPOWER_SLEEP(5*1000);
	pollTime(rtcZero, joinEpoch);
	return true;
}

void LORA_Module::pollTime(RTCZero *rtcZero, unsigned int joinEpoch) {
#ifdef SERIAL_DEBUG
	Serial.println("Preparing to poll time...");
#endif

	einkScreen->drawLoraSetup(3);
	uint8_t data_receive[64];
	int data_receive_maxSize = 64;

	int received = 0;
	for(int i = 0 ; i < 5 ; i ++){
#ifdef SERIAL_DEBUG
		Serial.println("Polling time...");
#endif
		if(!modem.available()){
			modem.poll();
			delay(2000);
		}

		while(modem.available() && received < data_receive_maxSize){
			data_receive[received] = modem.read();
			received++;
		}

		if(received > 0){
			break;
		}
		delay(10*1000);
	}

	if(received > 0){
		unsigned long time = 0;
		for(int i = 0; i < received ; i++){
			time = time << 8;
			time |= data_receive[i];
		}
		rtcZero->begin(false);
		time += rtcZero->getEpoch() - joinEpoch + 120;
		rtcZero->setEpoch(time);
#ifdef SERIAL_DEBUG
		Serial.print("Got time : "+String(time)+" or ");
		char buffer[64];
		time_t localTime = myTZ.toLocal(rtcZero->getEpoch());
		sprintf(buffer, "%02d/%02d/%d %02d:%02d:%02d",
				day(localTime),
				month(localTime),
				year(localTime),
				hour(localTime),
				minute(localTime),
				second(localTime));
		Serial.println(buffer);
#endif
		einkScreen->drawLoraSetup(4);
		LOWPOWER_SLEEP(5*1000);
	}
}

void LORA_Module::sendDataStore(LinkedList<CO2_data_t*> *dataList) {
	auto *payload = new uint8_t[dataList->size()*8+2];
	int payloadSize = dataList->size()*8+2;
	int dataListSize = dataList->size();

	payload[0] = dataListSize;
	pinMode(PIN_A1, INPUT);
	double batterie = analogRead(PIN_A1) * 2 * 3.3 / 1024;
	int batterieConv = (batterie * 10);
	batterieConv -= 25;
	payload[1] = batterieConv;

	CO2_data_t *currentCO2data;
	for(int i = 0 ; i < dataListSize ; i++){
		currentCO2data = dataList->shift();

		if(currentCO2data->mouvement > 15){
			currentCO2data->mouvement = 15;
		}

		payload[8*i+2] = currentCO2data->timestamp >> 24;
		payload[8*i+3] = currentCO2data->timestamp >> 16;
		payload[8*i+4] = currentCO2data->timestamp >> 8;
		payload[8*i+5] = currentCO2data->timestamp;

		int tempConv = (int)((currentCO2data->temperature + 25) * 10);
		payload[8*i+6] = tempConv >> 2;
		payload[8*i+7] = (tempConv << 6) | ((currentCO2data->humidite & 0x3F) >> 1);

		int co2Conv = currentCO2data->co2 / 10;
		payload[8*i+8] = ((currentCO2data->humidite & 0x3F) << 7) | (co2Conv >> 4);
		payload[8*i+9] = (co2Conv << 4);

		delete currentCO2data;
	}

#ifdef SERIAL_DEBUG
	Serial.print("Sending payload : ");
	for(int i = 0 ; i < payloadSize ; i ++){
		Serial.print(payload[i], HEX);
	}
	Serial.println("");
#endif

	modem.setPort(3);
	modem.beginPacket();
	modem.write(payload, payloadSize);
	modem.endPacket(true);
	modem.flush();

	delete[] payload;
}
