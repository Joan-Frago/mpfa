/*
    MPFA - Make Programming Fun Again

    For those of you who try to escape AI world, here you have a project to play on.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_LEN(array, type) sizeof(array) / sizeof(type)

#define SENSOR_NAME_SIZE 32

typedef enum SensorType
{
    ST_HTTP,
    ST_MODBUS,
} SensorType;

typedef struct SensorTypeHTTP
{
    char  addr[16];
    int   port;
} SensorTypeHTTP;

typedef struct SensorTypeModbus
{
    char  *addr;
} SensorTypeModbus;

typedef struct Sensor
{
    char        name[SENSOR_NAME_SIZE];
    SensorType  type;
    void *      type__; // Should find a better name
} Sensor;

Sensor CreateSensor(char *name, SensorType type)
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
            ((SensorTypeHTTP *)s.type__)->addr = "127.0.0.1";
            ((SensorTypeHTTP *)s.type__)->port = 8080;

            break;
        }
        case ST_MODBUS: break;
        default: printf("Unrecognized sensor type\n");
    }

    return s;
}

int main()
{
    Sensor sensors[] = {
        CreateSensor("HTTP Sensor", ST_HTTP),
        CreateSensor("Modbus Sensor", ST_MODBUS),
    };

    int i;
    for(i=0; i<ARRAY_LEN(sensors, Sensor); ++i)
    {

    }

    return 0;
}
