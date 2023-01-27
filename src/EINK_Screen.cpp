#include "EINK_Screen.h"

void EINK_Screen::drawMainFrame() {
	powerOn();

	display->clearBuffer();
	int width1 = display->width()/3;
	int width2 = display->width()*2/3-1;
	int width_full = display->width();
	int height = display->height()*0.15;
	int stroke_width = 3;

	display->fillRect(0, height - stroke_width, width_full, stroke_width*2, EPD_BLACK);

	stroke_width = 1;

	display->fillRect(width1-stroke_width, 0, stroke_width*2, height+stroke_width, EPD_BLACK);
	display->fillRect(width2-stroke_width, 0, stroke_width*2, height+stroke_width, EPD_BLACK);

	display->setTextColor(EPD_BLACK);
	display->setTextSize(2);
	display->setCursor(45, height/4+2);
	display->print(String(temperature, 1) + " C");
	display->drawBitmap(3, 3, epd_bitmap_temperature, 32, 32, EPD_BLACK);

	display->setCursor(width1 + 60, height/4);
	display->print(String(humidite) + " %");
	display->drawBitmap(width1+14, 3, epd_bitmap_humidity, 32, 32, EPD_BLACK);

	display->setCursor(width2 + 60, height/4);
	display->print(String(batterie) + " %");
	display->drawBitmap(width2+14, 3, epd_bitmap_battery, 32, 32, EPD_BLACK);

	uint16_t bar_color = EPD_RED;
	uint16_t text_color = EPD_WHITE;

	int upper_height = display->height() * 0.2;
	height = display->height()*0.4;
	width1 = 4;
	width2 = display->width()-8;
	stroke_width = 2;
	display->fillRect(width1, upper_height-stroke_width, width2, stroke_width*2, bar_color);
	display->fillRect(width1, upper_height+height-stroke_width, width2, stroke_width*2, bar_color);
	display->fillRect(width1-stroke_width, upper_height-stroke_width, stroke_width*2, height+stroke_width*2, bar_color);
	display->fillRect(width2, upper_height-stroke_width, stroke_width*2, height+stroke_width*2, bar_color);

	double percent = (double)(co2-minPPM)/(maxPPM-minPPM);
	if(percent > 1){
		percent = 1;
	}
	else if(percent < 0){
		percent = 0;
	}

	display->fillRect(width1, upper_height, width2*percent, height, bar_color);

	display->fillRect(0, display->height()*2/3-10, display->width()+10, display->height()/3, bar_color);

	display->setTextSize(5);
	display->setCursor(10, display->height()*3/4-5);
	display->setTextColor(text_color);
	display->print(String(co2)+" ppm");
	
	display->drawBitmap(display->width()*0.7, display->height()*5/8+5, epd_bitmap_co2_high, 96, 96, EPD_WHITE);

	display->display();

	powerOff();
}

void EINK_Screen::drawLoraSetup(int phase) {
	powerOn();

	display->clearBuffer();

	display->drawBitmap(16, 24, epd_bitmap_logo, 96, 96, EPD_BLACK);

	display->setTextSize(4);
	display->setTextColor(EPD_BLACK, EPD_WHITE);
	display->setCursor(128, display->height()/4-48);
	display->print("Monitoring");
	display->setCursor(display->width()*2/4-24, display->height()/4);
	display->setTextColor(EPD_RED, EPD_WHITE);
	display->setTextSize(8);
	display->print("CO2");

	int widthBar = display->width()-64;

	if(phase == 1){
		display->setTextColor(EPD_BLACK, EPD_WHITE);
		display->setCursor(28, display->height()*3/4-8);
		display->setTextSize(3);
		display->print("Initialisation LoRa");
		display->fillRect(32, display->height()/2+8, widthBar*0.1, 32, EPD_RED);
	}
	else if(phase == 2){
		display->setTextColor(EPD_BLACK, EPD_WHITE);
		display->setCursor(48, display->height()*3/4-8);
		display->setTextSize(3);
		display->print("LoRa initialise !");
		display->fillRect(32, display->height()/2+8, widthBar*0.3, 32, EPD_RED);
	}
	else if(phase == 3){
		display->setTextColor(EPD_BLACK, EPD_WHITE);
		display->setCursor(52, display->height()*3/4-8);
		display->setTextSize(3);
		display->print("Synchro. horloge");
		display->fillRect(32, display->height()/2+8, widthBar*0.5, 32, EPD_RED);
	}
	else if(phase == 4){
		display->setTextColor(EPD_BLACK, EPD_WHITE);
		display->setCursor(72, display->height()*3/4-8);
		display->setTextSize(2);
		RTCZero rtc;
		char buffer[64];
		time_t localTime = myTZ.toLocal(rtc.getEpoch());
		sprintf(buffer, "%02d/%02d/%d %02d:%02d:%02d",
				day(localTime),
				month(localTime),
				year(localTime),
				hour(localTime),
				minute(localTime),
				second(localTime));
		display->print(buffer);
		display->fillRect(32, display->height()/2+8, widthBar, 32, EPD_RED);
	}

	for(int i = 0 ; i < 4 ; i++){
		display->drawRect(32+i, display->height()/2+8+i, display->width()-64-i*2, 32-i*2, EPD_BLACK);
	}

	display->fillRect(0, display->height()*18/20-8, display->width(), display->height(), EPD_BLACK);
	display->setTextColor(EPD_WHITE);
	display->setTextSize(1);
	display->setCursor(8, display->height()*18/20);
	display->print("Nicolas Stein - Thomas Roussarie - Thanh Liem Ta");
	display->setCursor(72, display->height()*19/20);
	display->print("Shuchong Li - Hadrien Gourdet");

	display->setTextSize(3);
	display->setCursor(display->width()-96, display->height()*18/20);
	display->print("EISE5");

	display->display();

	powerOff();
}

void EINK_Screen::powerOff() {
	display->powerDown();
	delay(100);
	digitalWrite(EPD_ENA, LOW);
}

void EINK_Screen::powerOn() {
	digitalWrite(EPD_ENA, HIGH);
	delay(100);
	display->begin(THINKINK_TRICOLOR);
}

void EINK_Screen::setup() {
	pinMode(EPD_ENA, OUTPUT);
	digitalWrite(EPD_ENA, HIGH);
	display->begin(THINKINK_TRICOLOR);
	powerOff();
}

void EINK_Screen::setSensorValues(float new_temperature, int new_humidite, int new_co2) {
	temperature = new_temperature;
	humidite = new_humidite;
	co2 = new_co2;
}

