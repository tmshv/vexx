#include "timer.h"

BEGIN_PHYSICAL_NAMESPACE

timeval timer::oldTime;

void timer::reset()
    {
    gettimeofday(&oldTime, 0);
    }

pReal timer::time()
    {
    // find the current time from the system in floating point seconds
    struct timeval newTime;
    gettimeofday(&newTime, 0);

    long int secs =  newTime.tv_sec - oldTime.tv_sec;
    long int usecs = newTime.tv_usec - oldTime.tv_usec;

    pReal deltaTime = secs + ( usecs / 1000000.0f );

    return deltaTime;
    }

END_PHYSICAL_NAMESPACE
