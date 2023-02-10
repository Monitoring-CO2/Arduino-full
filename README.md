# Monitoring CO2 - Arduino-full

This program is a [Platform IO](https://platformio.org/) project tested on an Arduino : [MKR1310](https://docs.arduino.cc/hardware/mkr-wan-1310)

## Libraries used

- [MKRWAN](https://github.com/arduino-libraries/MKRWAN)
- [Arduino Low Power](https://github.com/arduino-libraries/ArduinoLowPower)
- [RTCZero](https://github.com/arduino-libraries/RTCZero)
- [Adafruit EPD](https://github.com/adafruit/Adafruit_EPD)
- [SparkFun SCD30 CO₂ Sensor](https://github.com/sparkfun/SparkFun_SCD30_Arduino_Library)
- [TimeZone](https://github.com/jchristensen/Timezone)
- [LinkedList](https://github.com/ivanseidel/LinkedList)
- [Edge Impulse](https://www.edgeimpulse.com)

## Overall operation of the program

The microcontroller first initiate the LoRaWAN module and join the network.  
When doing so, the backend server is notified and schedule a downlink message containing the current real world time.  
A few seconds after the join, the microcontroller tries to poll the downlink message and set its RTC clock with the real world time.  
When this is done, the microcontroller puts the CO2 sensor to sleep, by powering off the TPL, and put itself to sleep.  
It then enters its main loop, where it is put to sleep for 1s, then records the voltage of the motion sensor (HIGH, or LOW).  
Whenever the CO2 sensor is power on by the TPL (after some configurable time), the microcontroller also detects it and tries to record the current sensor values.  
The microcontroller updates the screen with the new values from the sensor (if it is daytime), and after recording a certain amount of data, it sends it over LoRa.  

## Program setup *(required)*

You **must** change certain headers (in the `src` folder) in order to configure the program :
 
- **CONFIG_PRIVATE.H**  
  *This file does not exist, you **need** to create it and fill it with these lines :*
  ```c++
  #define LORA_APP_EUI "<LORA_DEVICE_EUI>"
  #define LORA_APP_KEY "<LORA_DEVICE_APP_KEY>"
  ```
  *Don't forge the two `""` around the keys and don't put the `<` and `>`.*


- **CONFIG.H**  
  If you want to debug, you can uncomment the line
  ```c++
  //#define SERIAL_DEBUG
  ```
  When you enable debugging, it disables deep sleep features which increase power consumption by a lot.  
  **Do not enable debug in production**.