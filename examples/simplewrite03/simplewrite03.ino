// JDN @ AAU
// beer license http://jensd.dk/doc/beerlicense.txt
// Apr 2024

#include <espnowsimple.h>

/* simplewrite03 demonstrates how to send packager of differrent size between two nodes.
* 
* The packages all has a uint8_t byte in top of package which gives package type ID
*/

/* we just want to tx a struct 
* so you just stuff your data inside
* pls avoid Strings due to internal fragmentation due to use of heap
* Instead use

String myString; v = "hej" +"noget andet"; // etc
char dataAr[20];
myString.toCharArray(dataAr,20);
* and then tx dataAr

*/

/*
* packed directive packs the struct on byte aligment
*/
typedef struct myPackageType {
  int seqNr;
  uint8_t pkgTp;  // should b 1
  int i1;
  float f[3];
  char txt[5];
  // ...
} myPackageType;

typedef struct myPackageType2 {
  int seqNr;
  uint8_t pkgTp;  // should be 2
  float f;
  char txt[7];
  // ...
} myPackageType2;

// more to come if you need more package types

myPackageType txPkg, rxPkg;
myPackageType2 txPkg2, rxPkg2;

// getting my callbacks up and running
// seems to have problems if we do tx and rx at same tiem
// even if no package loss is detected

void espSimpleNowCallbackDataSent(const bool success) {
  //JDN Serial.print("tx(0 == ok) ");
  //Serial.println(scs);
}

/**
* the receiving function
* just copy the received package to local storage
* You might do it better by implementing a buffer system
*/

void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len) {
  // test
  // we do know the first element is uint8_t the ppkgTp;
  // you can check if missing packages - se simplewrite02 /JDN
  switch (*data) {
    case 1:  // myPackageType
      // extra chk if type an size fits together
      if (len == sizeof(myPackageType)) {
        rxPkg = *((myPackageType *)data);

        Serial.print("rx callback ok type 1 pkg: ");
        Serial.println(rxPkg.i1);
      }
      break;
    case 2:  // myPackageType2
      if (len == sizeof(myPackageType2)) {
        rxPkg2 = *((myPackageType2 *)data);

        Serial.print("rx callback ok type 2 pkg: ");
        Serial.println(rxPkg2.f);
      }
      break;
    default:  // MISSING
      Serial.print("unknown pkg, type field:");
      Serial.println(*data);
  }
}

//  max max 250 Bytes data acc expressif
//uint8_t espSimpleNowBroadcastData(char *data, uint16_t len)


void setup() {

  pinMode(5, OUTPUT);  // led pin on cansat NeXt

  Serial.begin(115200);
  delay(100);
  Serial.println("test");

  espSimpleNowInit(72);

  // init data
  // so callback rx can see different
  txPkg.seqNr = 0;
  txPkg.pkgTp = 1;
  txPkg.i1 = 40000;

  txPkg2.seqNr = 0;
  txPkg2.pkgTp = 2;
  txPkg2.f = 9111.24;
}

void txTest() {
  txPkg.i1++;
  txPkg.seqNr++;
  espSimpleNowBroadcastData((char *)(&txPkg), sizeof(myPackageType));

  txPkg2.f += 1.0;
  txPkg2.seqNr++;
  espSimpleNowBroadcastData((char *)(&txPkg2), sizeof(myPackageType2));


  digitalWrite(5, !digitalRead(5));
}

void loop() {
  txTest();  // comment this line if your are the RX part
  delay(2000);
}