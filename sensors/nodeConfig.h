#ifndef __NODECONFIG_H__
#define __NODECONFIG_H__

#define EEPROM_CONFIG_ADDRESS 0

enum nodeRole
{
    ROLE_NONE, ROLE_TEMPERATURE, ROLE_OPENING, ROLE_HUMIDITY, ROLE_LIGHT, ROLE_MOTION, ROLE_RAIN, ROLE_PRESSURE, ROLE_END
};

struct nodeEepromConfig
{
    uint8_t nodeId;
    uint8_t role[3];
    
    bool isValid()
    {
        bool result = true;
        result = result && nodeId != 255;
        for (int i = 0; i < 3; i++) {
            result = result && role[i] < ROLE_END;
        }
        
        return result;
    }
};

void nodeConfigListen(nodeEepromConfig *nodeConfig);
void nodeConfigRead(nodeEepromConfig *nodeConfig);
void nodeConfigInit(nodeEepromConfig *nodeConfig);
char* nodeGetRoleFromInt(uint8_t roleId);

#endif
