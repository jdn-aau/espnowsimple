// JDN @ AAU
// beer license http://jensd.dk/doc/beerlicense.txt
// Apr 2024

#include <espnowsimple.h>

// getting my callbacks
void espSimpleNowCallbackDataSent(const bool success) {
  uint8_t scs;
  scs = success;
  Serial.print("tx(0 == ok) ");
  Serial.println(scs);
}

// Yo shall provide a function with this name and parameters
// so the network can deliver data to you this way
// Here we do expect an int
void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len) {
  int i;
  i = *((int *)data);
  Serial.println(i);  // just dump the value
}

int testI = 0;


void setup() {
  Serial.begin(115200);
  delay(100);
  Serial.println("test");

  // init esp32 esp_now protocol use 72 as broadcast ID: See in expsimplenow.h to see the whole mac address
  espSimpleNowInit(72);
}

void txTest() {
  // lets send testI integer out in the sky :-)
  espSimpleNowBroadcastData((char *)(&testI), sizeof(int));

  testI++;

  delay(500);
}

void loop() {

  // ifs using this code as TX part uncomment the line below
  // otherwise this code it act as RX part


  //txTest();
}