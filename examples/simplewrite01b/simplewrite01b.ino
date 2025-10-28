
// JDN @ AAU
// beer license http://jensd.dk/doc/beerlicense.txt
// Apr 2024

// TX an Array of ints
#include <espnowsimple.h>

const int arSize = 10;

int txData[arSize], rxData[arSize];

int txCount = 0, rxCount = 0;

// getting my callbacks
void espSimpleNowCallbackDataSent(const bool success) {  
}

// we do expect one integer
void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len) {
  // chk if (arSize == len) {  // yes
  rxCount++;
  for (int i = 0; i < arSize; i++) {
    rxData[i] = txData[i];
  }
}

void setup() {
  pinMode(5, OUTPUT);  // led pin on cansat NeXt
  Serial.begin(115200);
  delay(100);
  Serial.println("test");

  espSimpleNowInit(72);
}

void txTest() {
  //lets send the array
  espSimpleNowBroadcastData((char *)(txData), arSize * sizeof(int));
  txCount++;

  digitalWrite(5, !digitalRead(5));
  delay(500);
}

void loop() {
  //txTest();
}