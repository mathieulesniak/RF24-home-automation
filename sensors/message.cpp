#include <Arduino.h>
#include <RF24.h>
#include "message.h"
#include "config.h"

bool sendMessage(uint8_t recipient, sensorMessagePayload* message, RF24 radio)
{
    radio.powerUp();
    radio.stopListening();
    radio.openWritingPipe(TO_ADDR(recipient));
    dumpPacketContent(*message);
    bool ok = radio.write(message, sizeof(*message));
    radio.startListening();
    if (ok) {
        printf_P(PSTR(" : ok\n"));
    } else {
        printf_P(PSTR(" : failed\n"));
    }
    
    return ok;
}

void dumpPacketContent(sensorMessagePayload payload)
{
  printf_P(PSTR("Packet #%lu, failed:%lu, batt:%i, sensor1:%i, sensor2:%i, sensor3:%i"), payload.sentPacketCounter, payload.failedPacketCounter, payload.batteryPercentage, payload.sensor[0], payload.sensor[1], payload.sensor[2]);
}