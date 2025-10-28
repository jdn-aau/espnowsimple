/* ----------------------------------------------------------------------------
* "THE BEER-WARE LICENSE":
* <jensd@jensd.dk/jdn@es.aau.dk> wrote this file.  As long as you retain this notice you
* can do whatever you want with this stuff. If we meet some day, and you think
* this stuff is worth it, you can buy me a beer in return.
* Jens Dalsgaard Nielsen April 2024
* ----------------------------------------------------------------------------
*
* Heavily inspired by
* Copyright (c) 2023 Samuli Nyman
* SPDX-License-Identifier: MIT
* http://cansat.fi
*/

#include <espnowsimple.h>

uint8_t broadcastMAC[6] = { 0x02, 0x2a, 0x56, 0x72, 0x2b, 0x00 };  // from cansat.fi
esp_now_peer_info_t peerInfo;
esp_err_t espNowSimpleErr;



uint8_t espSimpleNowRunning = false;


// callbacks required from esp-now library
// our interface - radio callback interface
// empty for now
//JDN __attribute__((weak)) void callbackDataReceived(String data) {}
//
// The following two you can(or must) overrule by your own

__attribute__((weak)) void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len) {}
__attribute__((weak)) void espSimpleNowCallbackDataSent(const bool success) {}



void internalDataSentCallback(const uint8_t *mac_addr, esp_now_send_status_t status) {
  bool success = status == ESP_NOW_SEND_SUCCESS;  //ESP_NOW_SE... == 0
  espSimpleNowCallbackDataSent(success);
}


void internalDataReceivedCallback(const unsigned char *mac, const unsigned char *incomingData, int len) {
  /*
  String receivedData = ""; cnrt to string
  for (int i = 0; i < len; i++) {
    receivedData += (char)incomingData[i];
  }

  JDN NASTY - call both ?
  JDN  onDataReceived(receivedData);  String... we dont want to pasre Strings ...
  */
  espSimpleNowCallbackBinRcvd(incomingData, len);
}

void setMacAddr(uint8_t lastByte, uint8_t mac[6]) {
  mac[0] = 0x02;   // NB it seems thats [0] must be even 
  mac[1] = 0x2A;
  mac[2] = 0x56;
  mac[3] = 0x72;
  mac[4] = 0x2B;
  mac[5] = lastByte;
}

uint8_t initESPNowMac(uint8_t *mac) {

  espSimpleNowRunning = false;

  WiFi.mode(WIFI_STA);

  // set receive address
  espNowSimpleErr = esp_wifi_set_mac(WIFI_IF_STA, broadcastMAC);
  if (ESP_OK != espNowSimpleErr) {
    return 1;
  }

  espNowSimpleErr = esp_now_init();
  if (ESP_OK != espNowSimpleErr) {
    return 2;
  }

  espNowSimpleErr = esp_wifi_set_protocol(WIFI_IF_STA, WIFI_PROTOCOL_LR);
  if (ESP_OK != espNowSimpleErr) {
    return 3;
  }

  espNowSimpleErr = esp_now_register_send_cb(internalDataSentCallback);
  if (ESP_OK != espNowSimpleErr) {
    return 4;
  }

  espNowSimpleErr = esp_now_register_recv_cb(internalDataReceivedCallback);
  if (ESP_OK != espNowSimpleErr) {
    return 5;
  }

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  espNowSimpleErr = esp_now_add_peer(&peerInfo);

  if (ESP_OK != espNowSimpleErr) {
    return 6;
  }

  espSimpleNowRunning = true;

  return 0;
}

uint8_t espSimpleNowInit(uint8_t macAddrLastByte) {

  //sanity chack last bit must not be 1 acc to expressif
  if (macAddrLastByte != ((macAddrLastByte >> 1) << 1)) {
    return -1;
  }

  setMacAddr(macAddrLastByte, broadcastMAC);

  return initESPNowMac(broadcastMAC);
}


//usr call II


uint8_t espSimpleNowBroadcastData(char *data, uint16_t len) {
  if (250 < len) {
    return -1;
  }

  if (espSimpleNowRunning) {
    return esp_now_send(broadcastMAC, (uint8_t *)data, len);
  } else {
    return -2;
  }
}

void espSimpleNowSetMacAddr(uint8_t mac[6]) {
  for (int i = 0; i < 6; i++) {
    broadcastMAC[i] = mac[i];
  }
}

void espSimpleNowGetMacAddr(uint8_t mac[6]) {
  for (int i = 0; i < 6; i++) {
    mac[i] = broadcastMAC[i];
  }
}
