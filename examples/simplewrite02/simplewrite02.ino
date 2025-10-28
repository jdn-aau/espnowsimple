// JDN @ AAU
// beer license http://jensd.dk/doc/beerlicense.txt
// Apr 2024

#include <espnowsimple.h>

/* we just want to tx a struct 
* so you just stuff your data inside
* pls avoid Strings due to internal fragmentation due to use of heap
* Instead use

String myString; v = "hej" +"noget andet"; // etc
char dataAr[20];
myString.toCharArray(dataAr,20);
* and then tx dataAr
*
*/

typedef struct myPackageType {
  int seqNr;  // you must maintain  yourself
  int i1;
  float f[3];
  char txt[5];
  // ...
} myPackageType;


myPackageType txPkg, rxPkg;



// getting my callbacks up and running
void espSimpleNowCallbackDataSent(const bool success) {
  uint8_t scs;
  scs = success;
  Serial.print("tx(0 == ok) ");
  Serial.println(scs);
}

void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len) {
  static int lastPkgNr = 0;
  int pkgLost;
  // test
  if (len == sizeof(myPackageType)) {
    rxPkg = *((myPackageType *)data);  // we can copy struct in one statement !
    Serial.print("rx callback ok: ");
    Serial.println(rxPkg.i1);
    pkgLost =  rxPkg.seqNr - lastPkgNr - 1;
    if (0 != pkgLost) {
      //no package lost since last time
    } else {
      // at least on pkg lost 
    }
    lastPkgNr = rxPkg.seqNr;  // rdy for next pkg
  } else {
    Serial.print("rx pkg bad size is/should be");
    Serial.print(len);
    Serial.print(" ");
    Serial.println(sizeof(myPackageType));
  }
}

//  max max 250 Bytes data acc expressif
//uint8_t espSimpleNowBroadcastData(char *data, uint16_t len)


uint8_t testMac[6] = { 1, 2, 3, 4, 5, 6 };

char testData[12];




void setup() {

  pinMode(5, OUTPUT);  // led pin on cansat NeXt

  Serial.begin(115200);
  delay(100);
  Serial.println("test simplewrite02");

  // init esp_now til lsb byte 72  (our "net)")
  espSimpleNowInit(72);

  // init data
  txPkg.seqNr = 100;  // just a number to TX
  txPkg.seqNr = 0;    // nothing received yet
}

void txTest() {
  txPkg.i1++;
  txPkg.seqNr++; // indicate we are tx next package 
  // you could fill din the rest of the struct it's up to you

  espSimpleNowBroadcastData((char *)(&txPkg), sizeof(myPackageType));

  digitalWrite(5, !digitalRead(5));  // toogle LED to indicate pkg tx'ed
}

void loop() {
  //txTest(); // uncomment if you are server
  delay(500);
}