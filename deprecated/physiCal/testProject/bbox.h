#ifndef BBOX_H
#define BBOX_H

#include "physiCal.h"
#include "jLib.h"

class bBox
    {
public:
    bBox( cal::particle &, cal::particle &, cal::particle &, cal::particle &,
          cal::particle &, cal::particle &, cal::particle &, cal::particle & );

    void update( JFLOAT );

private:
    cal::particle *_particles[8];
    };

#endif // BBOX_H
