#ifndef TIMER_H
#define TIMER_H

#include "physiCalGlobal.h"
#include "sys/time.h"

BEGIN_PHYSICAL_NAMESPACE

//! Basic class used for timing operations
class PHYSICAL_EXPORT timer
    {
public:
    //! reset this timer
    static void reset();
    //! find the time since reset was called
    static pReal time();
private:
    static struct timeval oldTime;
    timer();
    timer( const timer & );
    };

END_PHYSICAL_NAMESPACE

#endif
