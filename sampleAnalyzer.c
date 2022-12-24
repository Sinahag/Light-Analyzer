#include "includes/sampleAnalyzer.h"

static int num_samples_this_second;
static samplerDatapoint_t *record;

// Dips
static int num_dips;

// Voltage Variables
static double max_value;
static double min_value;
static double current_average;
static double previous_average = 0;

// Time Variables
static double max_time_interval;
static double min_time_interval;
static double sample_average_time;
static long end_time_for_last_sample = 0;

// Mutexes for reading/writing to voltage and time variables
static pthread_mutex_t voltage_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t elapsed_time_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t dips_mutex = PTHREAD_MUTEX_INITIALIZER;

// Internal Thread
static pthread_t analyzer_thread;

static void Analyzer_getMinMaxAverageVoltage()
{
    current_average = record[0].sampleInV;
    if (previous_average)
    {
        current_average = previous_average;
    }
    else
    {
        pthread_mutex_lock(&voltage_mutex);
        {
            max_value = record[0].sampleInV;
            min_value = record[0].sampleInV;
        }
        pthread_mutex_unlock(&voltage_mutex);
    }

    for (int i = 0; i < num_samples_this_second; i++)
    {
        current_average = WEIGHTING_ALPHA * (record[i].sampleInV) + (1 - WEIGHTING_ALPHA) * (current_average);
        pthread_mutex_lock(&voltage_mutex);
        {
            if (record[i].sampleInV > max_value)
            {
                max_value = record[i].sampleInV;
            }
            if (record[i].sampleInV < min_value)
            {
                min_value = record[i].sampleInV;
            }
        }
        pthread_mutex_unlock(&voltage_mutex);
    }
}

static void Analyzer_getMaxAndMinTimeInterval()
{
    long double sum = 0;
    double temp;
    if (end_time_for_last_sample)
    {
        pthread_mutex_lock(&elapsed_time_mutex);
        {
            temp = (record[0].timestampInNanoS - end_time_for_last_sample);
            if (temp > max_time_interval)
            {
                max_time_interval = record[0].timestampInNanoS - end_time_for_last_sample;
            }
            else if (temp < min_time_interval && temp > 0)
            {
                min_time_interval = record[0].timestampInNanoS - end_time_for_last_sample;
            }
        }
        pthread_mutex_unlock(&elapsed_time_mutex);

        sum += max_time_interval;
    }
    else
    {
        pthread_mutex_lock(&elapsed_time_mutex);
        {
            max_time_interval = 0;
            min_time_interval = 10000;
        }
        pthread_mutex_unlock(&elapsed_time_mutex);
    }
    for (int i = 1; i < num_samples_this_second; i++)
    {
        temp = record[i].timestampInNanoS - record[i - 1].timestampInNanoS;
        if (temp >= 0)
        {
            sum += temp;
            pthread_mutex_lock(&elapsed_time_mutex);
            {
                if (max_time_interval < temp)
                {
                    max_time_interval = temp;
                }
                if (min_time_interval > temp && temp > 0)
                {
                    min_time_interval = temp;
                }
            }
            pthread_mutex_unlock(&elapsed_time_mutex);
        }
    }

    end_time_for_last_sample = record[num_samples_this_second - 1].timestampInNanoS;
    sample_average_time = sum / num_samples_this_second;
}

static void Analyzer_countDips()
{
    num_dips = 0;
    double ambient_trigger = TRIGGER;
    for (int i = 1; i < num_samples_this_second; i++)
    {
        int value = record[i].sampleInV;
        int prev = record[i - 1].sampleInV;

        if ((value < (current_average - HYSTERESIS_V)) || (value > (current_average + HYSTERESIS_V)))
        {
            if ((value > (current_average - ambient_trigger)) & (prev < (current_average - ambient_trigger)))
            {
                pthread_mutex_lock(&dips_mutex);
                {
                    num_dips++; // only increase on the release of the cover
                }
                pthread_mutex_unlock(&dips_mutex);
            }else if ((value < (current_average + ambient_trigger)) & (prev > (current_average + ambient_trigger)))
            {
                pthread_mutex_lock(&dips_mutex);
                {
                    num_dips++; // only increase on the release of the cover
                }
                pthread_mutex_unlock(&dips_mutex);
            }
        }
    }
}

static void *Analyzer_getRecord()
{
    Analyzer_getMaxAndMinTimeInterval();
    Analyzer_getMinMaxAverageVoltage();
    Analyzer_countDips();
    printf("Interval ms (%.3lf, %.3lf) avg = %.3lf   Samples V (%.3f, %.3f) avg = %.3f   # Dips:  %d   # Samples:    %d\n",
           min_time_interval / 1000.0, max_time_interval / 1000.0, sample_average_time / 1000.0,
           (min_value / 4095) * 1.8, (max_value / 4095) * 1.8, (current_average / 4095) * 1.8,
           num_dips, num_samples_this_second);

    previous_average = current_average;
    return NULL;
}

double Analyzer_getMinV()
{
    pthread_mutex_lock(&voltage_mutex);
    double temp = (min_value / 4095) * 1.8;
    pthread_mutex_unlock(&voltage_mutex);
    return temp;
}

double Analyzer_getMaxV()
{
    pthread_mutex_lock(&voltage_mutex);
    double temp = (max_value / 4095) * 1.8;
    pthread_mutex_unlock(&voltage_mutex);
    return temp;
}

double Analyzer_getMinTime()
{
    pthread_mutex_lock(&elapsed_time_mutex);
    double temp = min_time_interval / 1000;
    pthread_mutex_unlock(&elapsed_time_mutex);
    return temp;
}

double Analyzer_getMaxTime()
{
    pthread_mutex_lock(&elapsed_time_mutex);
    double temp = max_time_interval / 1000;
    pthread_mutex_unlock(&elapsed_time_mutex);
    return temp;
}

int Analyzer_getNumDips()
{
    pthread_mutex_lock(&dips_mutex);
    int temp = num_dips;
    pthread_mutex_unlock(&dips_mutex);
    return temp;
}

void Analyzer_Start(samplerDatapoint_t *head, int size)
{
    record = head;
    num_samples_this_second = size;
    if (pthread_create(&analyzer_thread, NULL, &Analyzer_getRecord, NULL))
    {
        perror("Bad Thread");
    }
}

void Analyzer_Close()
{
    pthread_join(analyzer_thread, NULL);
}

void Analyzer_Cancel()
{
    pthread_cancel(analyzer_thread);
    pthread_mutex_destroy(&dips_mutex);
    pthread_mutex_destroy(&elapsed_time_mutex);
    pthread_mutex_destroy(&voltage_mutex);
}
