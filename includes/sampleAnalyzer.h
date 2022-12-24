#ifndef SAMPLE_ANALYZER_H
#define SAMPLE_ANALYZER_H

#define WEIGHTING_ALPHA 0.001
// Trigger of 0.1V to digital -> (val/4095)*1.8 = 0.1 so val is 227.5
#define TRIGGER 227.5
#define HYSTERESIS_V 68.25
// Hysteresis to 0.03V to digital -> (val/4095)*1.8 = 0.03 so val is 68.25
#include "sampleA2D.h"
#include "stdio.h"
#include "stdlib.h"

// starts analyzer on and processes features from data
void Analyzer_Start();

// Joins the current open thread and
void Analyzer_Close();

// Cancels the thread and frees mutexes
void Analyzer_Cancel();

// returns the minimum voltage of all historical samples
double Analyzer_getMinV();

// returns the maximum voltage of all historical samples
double Analyzer_getMaxV();

// returns the minimum sample time of all historical samples
double Analyzer_getMinTime();

// returns the maximum sample time of all historical samples
double Analyzer_getMaxTime();

// returns the number of dips in the current sampling period
int Analyzer_getNumDips();

#endif
