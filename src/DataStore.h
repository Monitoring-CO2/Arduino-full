#ifndef MONITORING_CO2_FULL_DATASTORE_H
#define MONITORING_CO2_FULL_DATASTORE_H

#include <LinkedList.h>
#include <RTCZero.h>

typedef struct {
	unsigned int timestamp;
	float temperature;
	int humidite;
	int co2;
	double mouvement;
} CO2_data_t;

class DataStore {
public:
	void addData(RTCZero *rtcZero, float temperature, int humidite, int co2, double mouvement);
	LinkedList<CO2_data_t*> *getDataList();
private:
	LinkedList<CO2_data_t*> dataList;
};


#endif //MONITORING_CO2_FULL_DATASTORE_H
