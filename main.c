/*
    MPFA - Make Programming Fun Again

    For those of you who try to escape AI world, here you have a project to play on.
*/

#include <stdio.h>

#include "mpfa.h"
#include "sensor.h"

int main()
{
    Sensor sensors[] = {
        SensorCreate("HTTP Sensor", ST_HTTP),
        SensorCreate("Modbus Sensor", ST_MODBUS),
    };

    int i;
    for(i=0; i<ARRAY_LEN(sensors, Sensor); ++i)
    {
        switch(sensors[i].type)
        {
            case ST_HTTP: 
            {
                printf(
                    "HTTP Sensor -> addr: %s port: %d\n",
                    SensorGetTypeHTTP(sensors[i])->addr,
                    SensorGetTypeHTTP(sensors[i])->port
                );
                break;
            }
            case ST_MODBUS:
            {
                printf(
                    "Modbus Sensor -> addr: %s\n",
                    SensorGetTypeModbus(sensors[i])->addr
                );

                break;
            }
            default: printf("Unrecognized sensor type\n");
        }
    }

    return 0;
}
