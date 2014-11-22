#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>


#include <Sync.h>
#include "printf.h"
#include "config.h"
#include <SPI.h>

#define WRITE_PIPE  ((uint8_t)0)
#define READ_PIPE   ((uint8_t)1)


#define TO_ADDR(x) (RF24_BASE_RADIO_ID + x)


RF24 radio(pinRfCse,pinRfCsn);

struct sensorMessagePayload      // 32 bytes max
{ 
    uint8_t messageFrom;
    uint32_t sentPacketCounter;
    uint32_t failedPacketCounter;
    byte batteryPercentage;      // 1 byte
    uint16_t sensor[3];
};




void getRadioData()
{
    static char floatBuffer[15];

    sensorMessagePayload payload;
    bool done = false;

    while (!done) {
      done =  radio.read(&payload, sizeof(payload));
    }


    printf_P(PSTR("Received packet from node : %i\n"), payload.messageFrom);
    printf_P(PSTR("Packet #%i\n"), payload.sentPacketCounter);
    printf_P(PSTR("Failed : %i\n"), payload.failedPacketCounter);
    printf_P(PSTR("- Battery : %i\n"), payload.batteryPercentage);
      
    for (int i = 0; i < 3; i++) {
        dtostrf(payload.sensor[i] / 100.00 ,8, 2, floatBuffer);
        printf_P(PSTR("- SENSOR %i : %s\n"), i, floatBuffer);
    }
    /*
        // First, stop listening so we can talk
        radio.stopListening();
        uint8_t zz = 22;
        RF24NetworkHeader headerAnswer(header.from_node);
        bool ok = network.write(headerAnswer, &zz, sizeof(zz));
        if (ok) {
            printf("Sent response.\n\r");
        } else {
            printf("Failed send response\n");
        }

        // Now, resume listening so we catch the next packets.
        radio.startListening();
    */

}



void setup(void)
{
    Serial.begin(57600);
    printf_begin();
    
    SPI.begin();
    radio.begin();
    radio.setAutoAck(true);
    radio.setChannel(90);
    radio.setDataRate(RF24_DATARATE);
    radio.openReadingPipe(READ_PIPE, TO_ADDR(0));
   // radio.openReadingPipe(WRITE_PIPE, TO_ADDR(0));
    radio.printDetails();
    radio.startListening();



    //network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{
    if (radio.available()) {
        getRadioData();
    }
}