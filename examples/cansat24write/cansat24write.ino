/*
*  CANSAT NeXT kit based cansat  - transmitter
*/

/* cansatdk
*  
* cansatdk.cpp
* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE":
* <jensd@jensd.dk/jdn@es.aau.dk> wrote this file.  As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return. 
* Jens Dalsgaard Nielsen April 2024
* ----------------------------------------------------------------------------
* be inspired here : https://randomnerdtutorials.com/get-change-esp32-esp8266-mac-address-arduino/
* and see http://jensd.dk/doc/esp32
*
*  from http://cansat.fi ...
*  This example shows how data can be sent from the satellite to the groundstation.
*  The same functions work the other way around as well, and can be used to send data to the satellite from the groundstation.
*  Caution - You should always have an antenna in a radio system before transmitting anything. Make sure the antenna is plugged in before sending data via the radio.
*
* Links:
  http://cansat.fi
* https://dronebotworkshop.com/esp-now/
*
* http://jensd.dk/doc/cansat/cansatdk  code etc for this page
*/



#include "CanSatNeXT.h"
#include "cansatdk.h"

#define NETID 72

#define LEDPIN 5

const unsigned long timePeriod = 500;  // sampling every 100 millisecond
unsigned long nextTime;

uint8_t nodeMAC[6];

void setup() {

  Serial.begin(115200);
  delay(100);

  pinMode(LEDPIN, OUTPUT);

  Serial.println("Jens 01");

  getMACAddr(nodeMAC);  // my personal ID

  // Initialize CanSatNeXT systems.
  // Calling this function with an address as a parameter enables the radio.
  // The address is shared by both the transmitter and receiver.

  CanSatInit(NETID);  // NB netID must not end on 1,3,5,7,9

  nextTime = millis() + timePeriod;
}



/*
cansatDK datapkg

typedef struct cansatdkPkg{
float accX,accY,accZ,
float gyroX,gyroY,gyroZ;
float pressure;
float temperature;
int seqNr;
uint8_t macAddr[6];
bool SDcardAvailable; 
} cansatdkPkg;
*/

cansatdkPkg dataPkg;
 
void fillDataPkg(cansatdkPkg *pkg) {
  static int mySeqNr = 0;
  readAcceleration((pkg->accX), (pkg->accY), (pkg->accZ));
  readGyro((pkg->gyroX), (pkg->gyroY), (pkg->gyroZ));
  pkg->pressure = readPressure();
  pkg->temperature = readTemperature();
  pkg->SDcardAvailable = SDCardPresent();
  mySeqNr++;
  memcpy(pkg->macAddr, nodeMAC, 6);
  pkg->seqNr = mySeqNr;
}

int iii = 0;

void loop() {

  if (nextTime <= millis()) {
    nextTime += timePeriod;

    Serial.println(iii);
    fillDataPkg(&dataPkg);  // measure all sensors and pack them

    sendData((char *)(&dataPkg), sizeof(dataPkg));
    digitalWrite(LEDPIN, !digitalRead(LEDPIN));
  }
}
