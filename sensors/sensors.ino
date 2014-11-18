#include "config.h"
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include "printf.h"
#include "nodeConfig.h"
#include "sensorsFunctions.h";


#ifdef VERSION_H
#include "version.h"
#else
const char program_version[] = "Unknown";
#endif


nodeEepromConfig myNode;





RF24 radio(9, 10);
// Network uses that radio
RF24Network network(radio);

// How often to send 'hello world to the other unit
const unsigned long interval = 2000; //ms

uint32_t failedPacketCounter = 0;
// When did we last send?
unsigned long last_sent;






// setup()
void setup() {
  Serial.begin(57600); // Initialisation du port série
  printf_begin();

  nodeConfigInit(&myNode);

  printf_P(PSTR("\n---------------------------------\n"));
  printf_P(PSTR("SENSOR NODE VERSION: %s\n"), program_version);
  nodeConfigRead(&myNode);
  printf_P(PSTR("| NodeId: %i\n"), myNode.nodeId);
  for (int i = 0; i < 3; i++) {
    printf_P(PSTR("| Role %i: %s\n"), i+1, nodeGetRoleFromInt(myNode.role[i]));
  }
  
  printf_P(PSTR("---------------------------------\n"));
  
  SPI.begin();
  radio.begin();
  radio.startListening();

  radio.printDetails();
 
  network.begin(/*channel*/ 90, /*node address*/ myNode.nodeId);
  
}

// loop()
void loop() {
  static uint32_t sentPacketCounter   = 0;
  

  if (Serial.available()) {
    char c = Serial.read();
    if (c == '@') {
      nodeConfigListen(&myNode);
    }
  }


  network.update();

  byte nodeId = 1;
  float temp;
  float pression;
  byte batteryPercentage;
  
  unsigned long now = millis();
  

  
  if (now - last_sent > interval) {

    last_sent = now;
    sensorBatteryLevel(&batteryPercentage);
    payload_t payload = {sentPacketCounter++, failedPacketCounter, batteryPercentage, temp, 0, 0};
    
    assignPayloadSensorValue(&myNode, &payload);
    

    
    
    printf_P(PSTR("Sending packet #%i (%s) : "), sentPacketCounter, strPacketContent(payload));
    printf_P(PSTR("TT : %s"), strPacketContent(payload));
    

    RF24NetworkHeader header(0);
    bool ok = network.write(header, &payload, sizeof(payload_t));
    
    if (ok) {
      printf_P(PSTR("OK\n\r"));
      failedPacketCounter = 0;
    } else {
      printf_P(PSTR("Failed\n\r"));
      failedPacketCounter++;
      printf_P(PSTR("NOW FAILED : %d"), failedPacketCounter);
      delay(250); // extra delay on fail to keep light on longer
    }
   
  }
}

char* strPacketContent(payload_t payload)
{
  char buffer[100];
  sprintf(
    buffer,
    "PACKET : %lu %lu %i %i %i %i",
    payload.sentPacketCounter,
    payload.failedPacketCounter,
    payload.batteryPercentage,
    payload.sensor[0],
    payload.sensor[1],
    payload.sensor[2]
    );
  printf_P(PSTR("DEBUG:%s"), buffer);
  Serial.println(payload.sensor[0]);
  return buffer;
}