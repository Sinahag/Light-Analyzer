#include "includes/joystick.h"

static double Joystick_getXReading()
{
    // Open file
    FILE *f = fopen(A2D_FILE_VOLTAGE2, "r");
    if (!f)
    {
        printf("ERROR: Unable to open voltage input file.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

static double Joystick_getYReading()
{ // Open file
    FILE *f = fopen(A2D_FILE_VOLTAGE3, "r");
    if (!f)
    {
        printf("ERROR: Unable to open voltage input file.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0)
    {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

int Joystick_getDirection()
{
    int x = Joystick_getXReading();
    int y = Joystick_getYReading();
    if (1024 < x && x < 3072)
    {
        if (y > 3072)
        {
            return LEFT;
        }
        else if (y < 1024)
        {
            return RIGHT;
        }
    }
    else if (1024 < y && y < 3072)
    {
        if (x > 3072)
        {
            return DOWN;
        }
        else
        {
            return UP;
        }
    }
    return NEUTRAL;
}
