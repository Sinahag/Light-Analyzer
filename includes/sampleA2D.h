#ifndef SAMPLEA2D_H
#define SAMPLEA2D_H

#define A2D_FILE_VOLTAGE1 "/sys/bus/iio/devices/iio:device0/in_voltage1_raw"
#define A2D_VOLTAGE_REF_V 1.8
#define A2D_MAX_READING 4095
#define A2D_MAXNUM_READINGS 1000

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct
{
    double sampleInV;
    long long timestampInNanoS;
} samplerDatapoint_t;

// Begin/end the background thread which samples light levels.
void Sampler_startSampling(void);
void Sampler_stopSampling(void);

// Get a copy of the samples in the sample history, removing values
// from our history here.
// Returns a newly allocated array and sets `length` to be the
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
// Note: function provides both data and size to ensure consistency.
samplerDatapoint_t *Sampler_extractAllValues(int *length);

// Returns how many valid samples are currently in the history.
int Sampler_getNumSamplesInHistory();

// prints the record of the current sample
void Sampler_getRecord();

// Get the total number of light level samples taken so far.
long long Sampler_getNumSamplesTaken(void);

#endif