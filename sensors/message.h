#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#define TO_ADDR(x) (RF24_BASE_RADIO_ID + x)


struct sensorMessagePayload      // 32 bytes max
{ 
    uint8_t messageFrom;
    uint32_t sentPacketCounter;
    uint32_t failedPacketCounter;
    byte batteryPercentage;      // 1 byte
    uint16_t sensor[3];
};


bool sendMessage(uint8_t recipient, sensorMessagePayload* message, RF24 radio);
void dumpPacketContent(sensorMessagePayload payload);

#endif