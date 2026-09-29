#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mpfa.h"
#include "sensor.h"

SensorTypeHTTP *SensorGetTypeHTTP(Sensor sensor)
{
    return ((SensorTypeHTTP *)sensor.type__);
}

SensorTypeModbus *SensorGetTypeModbus(Sensor sensor)
{
    return ((SensorTypeModbus *)sensor.type__);
}

Sensor SensorCreate(char *name, SensorType type)
{
    Sensor s = {0};

    if(strlen(name) > SENSOR_NAME_SIZE - 1)
        printf("Sensor name too long. Only using first %d characters\n", SENSOR_NAME_SIZE - 1);

    memset(s.name, 0, SENSOR_NAME_SIZE);
    strncpy(s.name, name, SENSOR_NAME_SIZE - 1);
    s.name[SENSOR_NAME_SIZE] = '\0';

    s.type = type;

    switch(type)
    {
        case ST_HTTP: 
        {
            s.type__ = (SensorTypeHTTP *)malloc(sizeof(SensorTypeHTTP));
            SensorGetTypeHTTP(s)->addr = "127.0.0.1";
            SensorGetTypeHTTP(s)->port = 8080;

            break;
        }
        case ST_MODBUS:
        {
            s.type__ = (SensorTypeModbus *)malloc(sizeof(SensorTypeModbus));
            SensorGetTypeModbus(s)->addr = "127.0.0.1";

            break;
        }
        default: printf("Unrecognized sensor type\n");
    }

    return s;
}
