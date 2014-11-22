#ifndef __CONFIG_H__
#define __CONFIG_H__

const int pinResetConfig  = 2;
const int pinTemperature  = 7; // Broche utilisée pour le bus 1-Wire
const int pinBattery      = A0;  // select the input pin for the battery sense point
const int pinRfCse        = 9;
const int pinRfCsn        = 10;

#define RF24_CHANNEL            90
#define RF24_DATARATE           RF24_250KBPS   //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
#define RF24_PA_LEVEL           RF24_PA_MAX
#define RF24_BASE_RADIO_ID      ((uint64_t)0xA8A8E1FC00LL)

const uint8_t sleepTimer    = 9; // 4s per sleep cycle
const uint8_t sleepCyclesBetweenTransmit = 1; //sleepTimer * sleepCyclesBetweenTransmit = total between 2 transmissions

#endif