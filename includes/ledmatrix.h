#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#define I2C_DEVICE_ADDRESS 0x20
#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"
#define CONFIG_P9_18 "config-pin P9_18 i2c"
#define CONFIG_P9_17 "config-pin P9_17 i2c"
#define TURNON_MATRIX "i2cset -y 1 0x70 0x21 0x00"
#define TURNON_NOFLASH "i2cset -y 1 0x70 0x81 0x00"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

// This function accepts an integer value between 0 and 99 (inclusive) and displays it on the LED Matrix
void displayInt(int integervalue);

// This function accepts a floating point value between 0 and 9.9 (inclusive) and displays it on the LED Matrix
void displayDouble(double decimalval);

#endif