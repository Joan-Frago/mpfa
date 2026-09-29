#define SENSOR_NAME_SIZE 32

typedef enum SensorType
{
    ST_HTTP,
    ST_MODBUS,
} SensorType;

typedef struct SensorTypeHTTP
{
    char  *addr;
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

/*
    Returns a pointer to a sensor's object type HTTP
*/
SensorTypeHTTP *SensorGetTypeHTTP(Sensor sensor);

/*
    Returns a pointer to a sensor's object type Modbus
*/
SensorTypeModbus *SensorGetTypeModbus(Sensor sensor);

/*
    Returns a copy to a new Sensor object
*/
Sensor SensorCreate(char *name, SensorType type);
