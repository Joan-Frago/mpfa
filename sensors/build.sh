#!/usr/bin/env bash

set -e

MOSQUITTO_CFLAGS=$(pkg-config --cflags libmosquitto)
MOSQUITTO_CLIBS=$(pkg-config --libs libmosquitto)

WARNINGS=(-Wall -Wextra -Wno-unused-parameter)

set -x

gcc -o generic-linux/sensor generic-linux/sensor.c "${WARNINGS[@]}" $MOSQUITTO_CFLAGS $MOSQUITTO_CLIBS
