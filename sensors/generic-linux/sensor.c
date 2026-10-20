#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mosquitto.h>
#include <unistd.h>

#define BROKER_ADDRESS "localhost"
#define BROKER_PORT 1883
#define TOPIC "home/sensor/temperature"

void on_connect(struct mosquitto *mosq, void *userdata, int result) {
    if (result == 0) {
        printf("[Sensor] Connected to server successfully\n");
    } else {
        printf("[Sensor] Connection failed with code %d\n", result);
    }
}

void on_disconnect(struct mosquitto *mosq, void *userdata, int result) {
    if (result == 0) {
        printf("[Sensor] Disconnected cleanly\n");
    } else {
        printf("[Sensor] Unexpected disconnection\n");
    }
}

void on_publish(struct mosquitto *mosq, void *userdata, int mid) {
    printf("[Sensor] Message with ID %d published successfully\n", mid);
}

int main(int argc, char *argv[]) {
    struct mosquitto *mosq = NULL;
    int rc;

    mosquitto_lib_init();

    mosq = mosquitto_new("sensor-client", true, NULL);
    if (!mosq) {
        fprintf(stderr, "Failed to create Mosquitto instance\n");
        return 1;
    }

    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_disconnect_callback_set(mosq, on_disconnect);
    mosquitto_publish_callback_set(mosq, on_publish);

    rc = mosquitto_connect(mosq, BROKER_ADDRESS, BROKER_PORT, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Failed to connect: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    rc = mosquitto_loop_start(mosq);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "Failed to start loop: %s\n", mosquitto_strerror(rc));
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
        return 1;
    }

    printf("Publishing temperature readings...\n");
    for (int i = 0; i < 10; i++) {
        char payload[50];
        snprintf(payload, sizeof(payload), "{\"temp\": 22.5, \"reading\": %d}", i);

        rc = mosquitto_publish(mosq, NULL, TOPIC, strlen(payload), payload, 1, false);
        if (rc != MOSQ_ERR_SUCCESS) {
            fprintf(stderr, "Failed to publish: %s\n", mosquitto_strerror(rc));
        } else {
            printf("Sent: %s\n", payload);
        }

        sleep(2);
    }

    mosquitto_loop_stop(mosq, true);
    mosquitto_disconnect(mosq);
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    printf("[Sensor] Done\n");
    return 0;
}
