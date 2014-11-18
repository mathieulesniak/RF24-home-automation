#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <RF24Network.h>
#include <RF24Network_config.h>
#include <Sync.h>
#include "printf.h"
#include "config.h"
#include <SPI.h>

RF24 radio(pinRfCse,pinRfCsn);

struct payload_t      // 32 bytes max
{ 
    uint32_t sentPacketCounter;
    uint32_t failedPacketCounter;
    byte batteryPercentage;      // 1 byte
    uint16_t sensor[3];
};


// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;


void getRadioData()
{
    RF24NetworkHeader header;
    static char floatBuffer[15];

    payload_t payload;
    bool done = false;

    while (!done) {
      done =  network.read(header, &payload, sizeof(payload));
    }

    printf_P(PSTR("Received packet from node : %i\n"), header.from_node);
    printf_P(PSTR("Packet #%i\n"), payload.sentPacketCounter);
    printf_P(PSTR("Failed : %i\n"), payload.failedPacketCounter);
    printf_P(PSTR("- Battery : %i\n"), payload.batteryPercentage);
      
    for (int i = 0; i < 3; i++) {
        dtostrf(payload.sensor[i] / 100.00 ,8, 2, floatBuffer);
        printf_P(PSTR("- SENSOR %i : %s\n"), i, floatBuffer);
     }

}



void setup(void)
{
    Serial.begin(57600);
    printf_begin();
    
    SPI.begin();
    radio.begin();
    radio.startListening();

    network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop(void)
{

    // Pump the network regularly
    network.update();
  
    // Is there anything ready for us?
    while (network.available()) {
      getRadioData();
    }
}