#ifndef __SENSORSFUNCTIONS_H__
#define __SENSORSFUNCTIONS_H__

#define DS18B20 0x28     // Adresse 1-Wire du DS18B20

void assignPayloadSensorValue(nodeEepromConfig* nodeConfig, sensorMessagePayload* payload);

bool sensorBatteryLevel(byte* Percentage);
bool sensorTemperature(uint16_t* temp);
bool sensorOpening();
bool sensorHumidity();
bool sensorLight();
bool sensorMotion();
bool sensorRain();
bool sensorPressure();

#endif
