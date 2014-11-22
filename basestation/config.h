#ifndef __CONFIG_H__
#define __CONFIG_H__

const int pinRfCse        = 9;
const int pinRfCsn        = 10;

#define RF24_CHANNEL            90
#define RF24_DATARATE           RF24_250KBPS   //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
#define RF24_PA_LEVEL           RF24_PA_MAX
#define RF24_BASE_RADIO_ID      ((uint64_t)0xA8A8E1FC00LL)

#endif