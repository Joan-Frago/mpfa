/*
    MPFA - Make Programming Fun Again

    For those of you who try to escape AI world, here you have a project to play on.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>

#include "mpfa.h"

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "home/sensor/temperature"

void on_connect(struct mosquitto *mosq, void *userdata, int result)
{
    if(result == 0)
    {
        printf("Connected to broker\n");
        mosquitto_subscribe(mosq, NULL, TOPIC, 1);
    }
    else
    {
        printf("Connection failed with error code %d\n", result);
    }
}

void on_disconnect(struct mosquitto *mosq, void *userdata, int result)
{
    if(result == 0)
        printf("Disconnected cleanly\n");
    else
        printf("Unexpected disconnection\n");
}

void on_message(struct mosquitto *mosq, void *userdata, const struct mosquitto_message *msg)
{
    if(msg->payloadlen)
        printf("Received on \"%s\": %s\n", msg->topic, (char *)msg->payload);
    else
        printf("Received empty message on \"%s\"\n", msg->topic);
}

void on_subscribe(struct mosquitto *mosq, void *userdata, int mid, int qos_count, const int *granted_qos)
{
    printf("Subscription successful (QoS granted: %d)\n", granted_qos[0]);
}

SensorTypeHTTP *SensorGetTypeHTTP(Sensor sensor)
{
    return ((SensorTypeHTTP *)sensor.type__);
}

SensorTypeModbus *SensorGetTypeModbus(Sensor sensor)
{
    return ((SensorTypeModbus *)sensor.type__);
}

Sensor SensorCreate(SensorCreateArgs args)
{
    Sensor s = {0};

    if(strlen(args.name) > SENSOR_NAME_SIZE - 1)
        printf("Sensor name too long. Only using first %d characters\n", SENSOR_NAME_SIZE - 1);

    memset(s.name, 0, SENSOR_NAME_SIZE);
    strncpy(s.name, args.name, SENSOR_NAME_SIZE - 1);
    s.name[SENSOR_NAME_SIZE] = '\0';

    s.type = args.type;

    switch(args.type)
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

int main()
{
    Sensor sensors[] = {
        SensorCreate((SensorCreateArgs){
            .name="HTTP Sensor",
            .type=ST_HTTP,
            .addr="127.0.0.1",
            .port=8080
        }),
        SensorCreate((SensorCreateArgs){
            .name="Modbus Sensor",
            .type=ST_MODBUS
        }),
    };

    int i;
    for(i=0; i<ARRAY_LEN(sensors); ++i)
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

    // --------------------------------------
    // --------------------------------------

    struct mosquitto *mosq = NULL;
    
    mosquitto_lib_init();

    mosq = mosquitto_new("subscriber-client", true, NULL);
    if(!mosq)
    {
        fprintf(stderr, "Failed to create mosquitto instance\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_subscribe_callback_set(mosq, on_subscribe);

    int rc;
    rc = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60);
    if(rc != MOSQ_ERR_SUCCESS)
    {
        fprintf(stderr, "Failed to connect: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    printf("Listening for messages on \"%s\"... (Ctrl+C to exit)\n", TOPIC);
    mosquitto_loop_forever(mosq, -1, 1);

    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    return 0;
}
