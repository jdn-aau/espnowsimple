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

#ifndef ESPSIMPLENOW

#define ESPSIMPLENOW


#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#define ESPNOW_SIMPLE_DATA_LGT 100

typedef struct  {
  uint8_t pkgTp;  // should
  uint32_t seqNr;;
  unsigned long time;
  char data [ESPNOW_SIMPLE_DATA_LGT];
}espNowSimplePkg;

extern esp_err_t espNowSimpleErr;  // Value 0 means noError !

extern uint8_t espSimpleNowRunning;  // false(0)) if not running, true running

// callbacks required from esp-now library
// our interface - radio callback interface
// empty for now


/**
 * The callback you shall implement as
 * to get data
 *
 * void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len)
 * {
 *   your code
 * }
 *

 */
__attribute__((weak)) void espSimpleNowCallbackBinRcvd(const uint8_t *data, int len);


/**
 * The callback you shall implement as
 * to check if data has been send
 * NB esp now do not guarantee if data is received in same order
 * as you did tx - ifff you are heavily loading the system
 *
 * void espSimpleNowCallbackDataSent(const bool success)
 * {
 * }
 *
 * to check if data has been send
 * to get data
 */
__attribute__((weak)) void espSimpleNowCallbackDataSent(const bool success);

/**
 * change last byte in MAC to
 * To used for broadcast
 * See esp now library
 * NB  parm must not n´edn on 1,3,5,7 (decimal)
 * or just lsb bit must not be 1
 * return:
 *   -1 if macaddressbyte i odd - must be even (lsb bit0 ==0)
 * rest of err codes comes from call to initESPNowMac
 *  1: esp_wifi_sec_mac  failed
 *  2: esp_now_init failed
 *  3: esp_wifi_set_protocol failed
 *  4: esp_now_register_send_cb failed
 *  5: esp_now_register_recv_cb failed
 *  6: esp_now_add_peer failed
 *
 *   espNowSimpleErr (of type esp_err_t) holds the actual errorcode
 *
 */
uint8_t espSimpleNowInit(uint8_t macAddrLastByte);


/**
 * You can change broadcast mac address
 *
 * Set new
 * */
void espSimpleNowSetMacAddr(uint8_t mac[6]);

/**
 * you can get current mac addr
 * @return 1: hej
 *
 * */
void espSimpleNowGetMacAddr(uint8_t mac[6]);

/**
 * Simple binary data tx
 * NB max size 250Byte
 * pls use max 249 just to be sure
 * @return 1: well.
 * */
uint8_t espSimpleNowBroadcastData(char *data, uint16_t len);


#endif
