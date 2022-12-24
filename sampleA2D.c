#include "includes/sampleA2D.h"

static samplerDatapoint_t records[A2D_MAXNUM_READINGS];
static int num_readings = 0;
static int num_samples_taken = 0;
static pthread_t samplerthread;

samplerDatapoint_t *Sampler_extractAllValues(int *length)
{
    num_readings++;
    *length = num_samples_taken;
    num_samples_taken = 0;
    return records;
}

double getVoltage1Reading()
{
    // Open file
    FILE *f = fopen(A2D_FILE_VOLTAGE1, "r");
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

void *Sampler_sample()
{
    struct timeval timestamp;
    struct timespec sleeptime;
    sleeptime.tv_nsec = 1000000;
    sleeptime.tv_sec = 0.001;
    for (int i = 0; i < A2D_MAXNUM_READINGS; i++)
    {
        records[i].sampleInV = getVoltage1Reading();
        gettimeofday(&timestamp, NULL);
        records[i].timestampInNanoS = timestamp.tv_usec;
        num_samples_taken++;
        nanosleep(&sleeptime, &sleeptime); // sleep 1 ms every sample
    }
    return NULL;
}

void Sampler_startSampling(void)
{
    pthread_create(&samplerthread, NULL, &Sampler_sample, NULL);
}

void Sampler_stopSampling(void)
{
    pthread_cancel(samplerthread);
}

long long Sampler_getNumSamplesTaken(void)
{
    return num_samples_taken;
}

int Sampler_getNumSamplesInHistory()
{
    return num_readings;
}
