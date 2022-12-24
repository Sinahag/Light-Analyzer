#include "includes/interface.h"

static pthread_t interface_thread;
static pthread_mutex_t cancel_mutex = PTHREAD_MUTEX_INITIALIZER;
static bool isCancelled = false;

void Interface_Cancel()
{
    pthread_cancel(interface_thread);
}

bool Interface_isCancelled()
{
    return isCancelled;
}

static void *Interface_listenForJoy()
{
    while (1)
    {
        switch (Joystick_getDirection())
        {
        case UP:
            // display max voltage
            displayDouble(Analyzer_getMaxV());
            break;
        case RIGHT:
            // display max interval
            displayDouble(Analyzer_getMaxTime());
            break;
        case DOWN:
            // display min voltage
            displayDouble(Analyzer_getMinV());
            break;
        case LEFT:
            // dispay min interval
            displayDouble(Analyzer_getMinTime());
            break;

        default:
            displayInt(Analyzer_getNumDips());
        }
        if (Button_readFromButton() == 0)
        {
            printf("Shutting down...\n");
            Sampler_stopSampling();
            Analyzer_Cancel();
            pthread_mutex_lock(&cancel_mutex);
            {
                isCancelled = true;
            }
            pthread_mutex_unlock(&cancel_mutex);
            Interface_Cancel();
            return NULL;
        }
    }
    return NULL;
}

void Interface_Start()
{
    Button_initialize();
    if (pthread_create(&interface_thread, NULL, &Interface_listenForJoy, NULL))
    {
        perror("Bad Thread");
    }
}

void Interface_Close()
{
    pthread_join(interface_thread, NULL);
}
