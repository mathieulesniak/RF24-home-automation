#ifndef __CONFIG_H__
#define __CONFIG_H__

const int pinResetConfig  = 2;
const int pinTemperature  = 7; // Broche utilisée pour le bus 1-Wire
const int pinBattery      = A0;  // select the input pin for the battery sense point
const int pinRfCse        = 9;
const int pinRfCsn        = 10;

const uint8_t sleepTimer    = 9; // 4s per sleep cycle
const uint8_t sleepCyclesBetweenTransmit = 2; //sleepTimer * sleepCyclesBetweenTransmit = total between 2 transmissions

#endif