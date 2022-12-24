#include "includes/ledmatrix.h"

// array index also reflects the number being represented
static unsigned char left_digit[10][8] = {
    {0x00, 0x20, 0x50, 0x50, 0x50, 0x50, 0x50, 0x20},
    {0x00, 0x70, 0x20, 0x20, 0x20, 0x20, 0x60, 0x20},
    {0x00, 0x70, 0x40, 0x20, 0x20, 0x10, 0x50, 0x20},
    {0x00, 0x60, 0x10, 0x10, 0x20, 0x10, 0x10, 0x60},
    {0x00, 0x10, 0x10, 0x70, 0x50, 0x50, 0x30, 0x10},
    {0x00, 0x20, 0x50, 0x10, 0x70, 0x40, 0x40, 0x70},
    {0x00, 0x20, 0x50, 0x50, 0x60, 0x40, 0x40, 0x30},
    {0x00, 0x20, 0x20, 0x20, 0x10, 0x10, 0x10, 0x70},
    {0x00, 0x20, 0x50, 0x50, 0x20, 0x50, 0x50, 0x20},
    {0x00, 0x10, 0x10, 0x10, 0x30, 0x50, 0x50, 0x20}};

static unsigned char right_digit[10][8] = {
    {0x00, 0x01, 0x82, 0x82, 0x82, 0x82, 0x82, 0x01},
    {0x00, 0x83, 0x01, 0x01, 0x01, 0x01, 0x03, 0x01},
    {0x00, 0x83, 0x02, 0x01, 0x01, 0x80, 0x82, 0x01},
    {0x00, 0x03, 0x80, 0x80, 0x01, 0x80, 0x80, 0x03},
    {0x00, 0x80, 0x80, 0x83, 0x82, 0x82, 0x81, 0x80},
    {0x00, 0x01, 0x82, 0x80, 0x83, 0x02, 0x02, 0x83},
    {0x00, 0x01, 0x82, 0x82, 0x03, 0x02, 0x02, 0x81},
    {0x00, 0x01, 0x01, 0x01, 0x80, 0x80, 0x80, 0x83},
    {0x00, 0x01, 0x82, 0x82, 0x01, 0x82, 0x82, 0x01},
    {0x00, 0x80, 0x80, 0x80, 0x81, 0x82, 0x82, 0x01}};

static unsigned char decimal_point[8] =
    {0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
static unsigned int i2cFileDesc = 0;

static void runCommand(char *command)
{
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe))
    {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
        // printf("--> %s", buffer); // Uncomment for debugging
    }
    // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0)
    {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
    }
}

static void initI2cBus(char *bus, int address)
{
    runCommand(CONFIG_P9_17);
    runCommand(CONFIG_P9_18);
    runCommand(TURNON_MATRIX);
    runCommand(TURNON_NOFLASH);

    i2cFileDesc = open(bus, O_RDWR);
    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if (result < 0)
    {
        perror("I2C: Unable to set I2C device to slave address.");
        exit(1);
    }
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr,
                        unsigned char value) // pass in 1 byte (2 hex digits)
{
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if (res != 2)
    {
        perror("I2C: Unable to write i2c register.");
        exit(1);
    }
}

void displayInt(int integervalue)
{
    if (!i2cFileDesc)
    {
        initI2cBus(I2CDRV_LINUX_BUS1, 0x70);
    }
    if (integervalue > 99)
    {
        integervalue = 99;
    }
    int ones = integervalue % 10;
    int tens = integervalue / 10;
    unsigned char displayBits[8];

    for (int i = 0; i < 8; i++)
    {
        displayBits[i] = left_digit[tens][i] | right_digit[ones][i];
    }

    int i = 0; // iterate through the bit-mapped arrays
    for (unsigned char addr_iterate = 0x0; addr_iterate < 0xF; addr_iterate += 0x2)
    {
        writeI2cReg(i2cFileDesc, addr_iterate, displayBits[i]);
        i++;
    }
}

void displayDouble(double decimalval)
{
    if (!i2cFileDesc)
    {
        initI2cBus(I2CDRV_LINUX_BUS1, 0x70);
    }
    if (decimalval > 10.0)
    {
        decimalval = 9.9;
    }
    int ones = (int)decimalval % 10;
    int decimal = (int)(decimalval * 10) % 10;
    unsigned char displayBits[8];

    for (int i = 0; i < 8; i++)
    {
        displayBits[i] = left_digit[ones][i] | right_digit[decimal][i] | decimal_point[i];
    }

    int i = 0; // iterate through the bit-mapped arrays
    for (unsigned char addr_iterate = 0x0; addr_iterate < 0xF; addr_iterate += 0x2)
    {
        writeI2cReg(i2cFileDesc, addr_iterate, displayBits[i]);
        i++;
    }
}
