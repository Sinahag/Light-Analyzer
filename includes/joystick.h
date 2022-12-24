#ifndef JOYSTICK_H
#define JOYSTICK_H

#define A2D_FILE_VOLTAGE2 "/sys/bus/iio/devices/iio:device0/in_voltage2_raw"
#define A2D_FILE_VOLTAGE3 "/sys/bus/iio/devices/iio:device0/in_voltage3_raw"

#define NEUTRAL 0
#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

// returns an integer referring to the joystick direction
int Joystick_getDirection();

#endif