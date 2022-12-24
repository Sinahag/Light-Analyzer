#ifndef INTERFACE_H
#define INTERFACE_H

#include "joystick.h"
#include "button.h"
#include "ledmatrix.h"
#include "sampleAnalyzer.h"
#include <stdbool.h>

// starts reading from the joystick and signals what to display
void Interface_Start();

// close the running process and free
void Interface_Close();

// cancels the running process and frees memory
void Interface_Cancel();

// returns whether the interface has been shutdown or not
bool Interface_isCancelled();

#endif