#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <Arduino.h>
#include "eepromUpdateBlock.h";
#include "nodeConfig.h"

String readConsoleData = "";
byte currentCommand = 0;


void nodeConfigListen(nodeEepromConfig* myNode)
{
    uint8_t nodeId;
    uint8_t nodeRole;
    char curCharCmd;

    printf_P(PSTR("> CONFIG MODE\n"));
    while (true) {
        while (Serial.available()) {
            char c = Serial.read();
            if (c == '\n') {
                readConsoleData.trim();
                curCharCmd = readConsoleData.charAt(0);
                switch (curCharCmd) {
                    case 'N':
                        if (readConsoleData.substring(1) == "?") {
                            printf_P(PSTR("> nodeId = %i\n"), myNode->nodeId);
                        } else {
                            nodeId = readConsoleData.substring(1).toInt();
                            if (nodeId > 0 && nodeId <= 255) {
                                myNode->nodeId = nodeId;
                              
                                eeprom_update_block(myNode, EEPROM_CONFIG_ADDRESS, sizeof(*myNode));
                                printf_P(PSTR("Setting nodeId to %d\n"), myNode->nodeId);
                            } else {
                                printf_P(PSTR("Invalid nodeId %d\n"), nodeId);
                            }
                        }
                        break;
                    case 'A':
                    case 'B':
                    case 'C':
                    
                        if (readConsoleData.substring(1) == "?") {
                            printf_P(PSTR("ROLE %c : %s\n"), curCharCmd, nodeGetRoleFromInt(myNode->role[curCharCmd - 'A']));
                        } else {
                            nodeRole = readConsoleData.substring(1).toInt();
                            if (nodeRole >= ROLE_NONE && nodeRole < ROLE_END) {
                                myNode->role[curCharCmd - 'A'] = nodeRole;
                                eeprom_update_block(myNode, EEPROM_CONFIG_ADDRESS, sizeof(*myNode));
                                printf_P(PSTR("Setting ROLE %c to %s\n"), curCharCmd, nodeGetRoleFromInt(myNode->role[curCharCmd - 'A']));
                            } else {
                                printf_P(PSTR("Invalid role %d\n"), nodeId);
                            }
                        }
                        break;
                    default:
                        printf_P(PSTR("Invalid command\n"));
                }

                readConsoleData = "";
            } else {
                readConsoleData += c;
                Serial.print(c);
            }
        }
    }
}

void nodeConfigRead(nodeEepromConfig* myNode)
{
    eeprom_read_block(myNode, EEPROM_CONFIG_ADDRESS, sizeof(*myNode));
    
    if (!myNode->isValid()) {
        nodeConfigListen(myNode);
    }
}

void nodeConfigInit(nodeEepromConfig* myNode)
{
    myNode->nodeId = 0;
    for (int i = 0;i < 3;i++) {
        myNode->role[i] = ROLE_NONE;
    }
}

char* nodeGetRoleFromInt(uint8_t nodeRole)
{
    switch (nodeRole) {
        case ROLE_NONE:
            return "NONE";
            break;
        case ROLE_TEMPERATURE:
            return "TEMPERATURE";
            break;
        case ROLE_OPENING:
            return "OPENING";
            break;
        case ROLE_HUMIDITY:
            return "HUMIDITY";
            break;
        case ROLE_LIGHT:
            return "LIGHT";
            break;
        case ROLE_MOTION:
            return "MOTION";
            break;
        case ROLE_RAIN:
            return "RAIN";
            break;
        case ROLE_PRESSURE:
            return "PRESSURE";
            break;
        default:
            return "UNKNOWN ROLE";
            break;
    }
}
