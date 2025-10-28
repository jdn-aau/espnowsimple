/*
*  CANSAT NeXT kit base example
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
*
*  This example shows how data can be sent from the satellite to the groundstation.
*  The same functions work the other way around as well, and can be used to send data to the satellite from the groundstation.
*  Caution - You should always have an antenna in a radio system before transmitting anything. Make sure the antenna is plugged in before sending data via the radio.
*
* Links
* 
*  http://cansat.fi 
* https://dronebotworkshop.com/esp-now/
*
* http://jensd.dk/doc/cansat/cansatdk  this code can be found here
*/

#include "CanSatNeXT.h"
#include "cansatdk.h"
 
#define NETID 72


// 2 for base  5 for cansat NeXT
#define BASELED 5    

void setup() {

  Serial.begin(115200);
  pinMode(BASELED, OUTPUT);

  // Initialize CanSatNeXT systems. 
  // Calling this function with an address as a parameter enables the radio. 
  // The address is shared by both the transmitter and receiver.
   GroundStationInit(NETID);
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


void printMAC(uint8_t *mac)
{
  for (int i = 0; i < 6; i++) {
    Serial.print(" Ox");
    if (mac[i] <= 9)
      Serial.print("0");
    Serial.print(mac[i], HEX);
  }
  Serial.println();
}


void displayData(cansatdkPkg *data)
{
  Serial.print("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nNodeID: "); // kan fjernes
  printMAC(data->macAddr);
  Serial.println("nr " + (String)(data->seqNr));
  Serial.println("acc " + (String)(data->accX) + " " + (String)(data->accY) + " " + (String)(data->accZ));
  Serial.println("gyr " + (String)(data->gyroX) + " " + (String)(data->gyroY) + " " + (String)(data->gyroZ));
  Serial.println("prs "  + (String(data->pressure)));
  Serial.println("tmp "  + (String(data->temperature)));
}

void onBinaryDataReceived(const uint8_t *data, int len){
  memcpy(&dataPkg, data, sizeof(dataPkg));
  Serial.println(dataPkg.seqNr);
  displayData(&dataPkg);
}
 
 
void loop() {
}
