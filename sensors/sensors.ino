#include "config.h"
#include <RF24.h>
#include <SPI.h>

#include "printf.h"
#include "nodeConfig.h"
#include "message.h"
#include "sensorsFunctions.h"
#include "sleep.h"


#ifdef VERSION_H
#include "version.h"
#else
const char program_version[] = "Unknown";
#endif


nodeEepromConfig myNodeConfig;
 


#define WRITE_PIPE  ((uint8_t)0)
#define READ_PIPE   ((uint8_t)1)


RF24 radio(pinRfCse, pinRfCsn);


uint32_t failedPacketCounter = 0;
uint32_t sentPacketCounter   = 0;





// setup()
void setup() {
  Serial.begin(57600); // Initialisation du port série
  printf_begin();

  nodeConfigInit(&myNodeConfig);

  printf_P(PSTR("\n---------------------------------\n"));
  printf_P(PSTR("SENSOR NODE VERSION: %s\n"), program_version);
  nodeConfigRead(&myNodeConfig);
  printf_P(PSTR("| NodeId: %i\n"), myNodeConfig.nodeId);
  for (int i = 0; i < 3; i++) {
    printf_P(PSTR("| Role %i: %s\n"), i+1, nodeGetRoleFromInt(myNodeConfig.role[i]));
  }
  
  printf_P(PSTR("---------------------------------\n"));
  
  sleepSetup(sleepTimer, sleepCyclesBetweenTransmit);
  SPI.begin();
  
  radio.begin();
  radio.setAutoAck(true);
  radio.setChannel(RF24_CHANNEL);
  radio.setDataRate(RF24_DATARATE);
  radio.openWritingPipe(TO_ADDR(0));

  radio.openReadingPipe(READ_PIPE, TO_ADDR(myNodeConfig.nodeId));
  radio.startListening();

  radio.printDetails();
}

void wakeUp ()
{
   // detachInterrupt(0);

}

void loop() {
    //attachInterrupt(0, wakeUp, LOW);
    if (Serial.available()) {
      char c = Serial.read();
      if (c == '@') {
        nodeConfigListen(&myNodeConfig);
      }
    }

    sensorMessagePayload payload = {myNodeConfig.nodeId, sentPacketCounter++, failedPacketCounter};
    
    assignPayloadSensorValue(&myNodeConfig, &payload);

    
    if (sendMessage(0, &payload, radio)) {
        failedPacketCounter = 0;

        // Wait here until we get a response, or timeout (250ms)
        unsigned long started_waiting_at = millis();
        bool timeout = false;
        while (!radio.available() && !timeout) {
            if (millis() - started_waiting_at > 200 ) {
                timeout = true;
            }
        }

        // Describe the results
        if (!timeout) {
           // got a message from base, parse it
        }

    } else {
        printf_P(PSTR("Failed\n"));
        failedPacketCounter++;
    }
  
    printf_P(PSTR("Going to sleep\n"));
    Serial.flush();
    radio.powerDown();
    goToSleep();
}

