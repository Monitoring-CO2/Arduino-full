#include "DataStore.h"

void DataStore::addData(RTCZero *rtcZero, float temperature, int humidite, int co2, double mouvement) {
	auto *co2Data = new CO2_data_t;

	Serial.println("current timestamp : "+String(rtcZero->getEpoch()));

	co2Data->timestamp = rtcZero->getEpoch();
	co2Data->temperature = temperature;
	co2Data->humidite = humidite;
	co2Data->co2 = co2;
	co2Data->mouvement = mouvement;
	dataList.add(co2Data);
}

LinkedList<CO2_data_t*> *DataStore::getDataList() {
	return &dataList;
}
