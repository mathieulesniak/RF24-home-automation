#ifndef __SENSORSFUNCTIONS_H__
#define __SENSORSFUNCTIONS_H__

#define DS18B20 0x28     // Adresse 1-Wire du DS18B20

struct payload_t      // 32 bytes max
{ 
  uint32_t sentPacketCounter;
  uint32_t failedPacketCounter;
  byte batteryPercentage;      // 1 byte
  uint16_t sensor[3];
};

void assignPayloadSensorValue(nodeEepromConfig* nodeConfig, payload_t* payload);

bool sensorBatteryLevel(byte* Percentage);
bool sensorTemperature(uint16_t* temp);
bool sensorOpening();
bool sensorHumidity();
bool sensorLight();
bool sensorMotion();
bool sensorRain();
bool sensorPressure();

#endif
