#ifndef BUTTON_H
#define BUTTON_H
#define BUTTON_CONFIG "config-pin p8.43 gpio"
#define USER_PORT_DIRECTION "echo in > /sys/class/gpio/gpio72/direction"
#define USER_PORT_ACTIVE "echo 0 > /sys/class/gpio/gpio72/active_low"
#define USER_PORT "/sys/class/gpio/gpio72/value"
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>

// initializes the button direction, active_low and enables it
void Button_initialize();

// reads the current value from the button (active_low) -> 0 == pressed
int Button_readFromButton();

#endif
