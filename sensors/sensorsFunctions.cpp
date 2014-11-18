#include <Arduino.h>
#include <OneWire.h> // Inclusion de la librairie OneWire
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP085_U.h>

#include "config.h"
#include "nodeConfig.h"

#include "sensorsFunctions.h"

// Based upon http://skyduino.wordpress.com/2012/04/26/arduino-capteur-de-temperature-ds18b20/
bool sensorTemperature(uint16_t *temp)
{
    OneWire ds(pinTemperature); // Création de l'objet OneWire ds

    byte data[9], addr[8];
    // data : Données lues depuis le scratchpad
    // addr : adresse du module 1-Wire détecté

    if (!ds.search(addr)) { // Recherche un module 1-Wire
        ds.reset_search();    // Réinitialise la recherche de module
        printf_P(PSTR("SEARCH FAILED\n"));
        return false;         // Retourne une erreur
    }

    if (OneWire::crc8(addr, 7) != addr[7]) {// Vérifie que l'adresse a été correctement reçue
        printf_P(PSTR("address FAILED\n"));
        return false;                        // Si le message est corrompu on retourne une erreur
    }

    if (addr[0] != DS18B20) {// Vérifie qu'il s'agit bien d'un DS18B20
        printf_P(PSTR("NOT A DS\n"));
        return false;         // Si ce n'est pas le cas on retourne une erreur
    }

    ds.reset();             // On reset le bus 1-Wire
    ds.select(addr);        // On sélectionne le DS18B20

    ds.write(0x44, 1);      // On lance une prise de mesure de température
    delay(800);             // Et on attend la fin de la mesure

    ds.reset();             // On reset le bus 1-Wire
    ds.select(addr);        // On sélectionne le DS18B20
    ds.write(0xBE);         // On envoie une demande de lecture du scratchpad

    for (byte i = 0; i < 9; i++) {// On lit le scratchpad
        data[i] = ds.read();       // Et on stock les octets reçus
    }
    
    // Calcul de la température en degré Celsius
    *temp = 100 * (((data[1] << 8) | data[0]) * 0.0625); 
    
    // Pas d'erreur
    return true;
}

bool sensorBatteryLevel(byte *percentage)
{
  int sensorValue = analogRead(pinBattery);
  //Serial.println(sensorValue);
   
   // 1M, 470K divider across battery and using internal ADC ref of 1.1V
   // Sense point is bypassed with 0.1 uF cap to reduce noise at that point
   // ((1e6+470e3)/470e3)*1.1 = Vmax = 3.44 Volts
   // 3.44/1023 = Volts per bit = 0.003363075
   float batteryV  = sensorValue * 0.003363075;
   *percentage = sensorValue / 10;
   printf_P(PSTR("- Batt : %d\n"), batteryV);
   
}

bool sensorOpening()
{

}

bool sensorHumidity()
{

}

bool sensorLight()
{

}

bool sensorMotion()
{

}

bool sensorRain()
{

}

bool sensorPressure()
{

}

void assignPayloadSensorValue(nodeEepromConfig* nodeConfig, payload_t* payload)
{
    for (uint8_t i = 0; i < sizeof(nodeConfig->role); i++) {
        switch (nodeConfig->role[i]) {
            case ROLE_TEMPERATURE:
                if (!sensorTemperature(&payload->sensor[i])) {
                    printf_P(PSTR("FAILED TO GET TEMP\n"));
                }
                break;
            case ROLE_OPENING:
                break;
            case ROLE_HUMIDITY:
                break;
            case ROLE_LIGHT:
                break;
            case ROLE_MOTION:
                break;
            case ROLE_RAIN:
                break;
            case ROLE_PRESSURE:
                break;

        }
    }
}
