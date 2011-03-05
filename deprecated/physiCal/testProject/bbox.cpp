#include "bbox.h"

bBox::bBox( cal::particle &a, cal::particle &b, cal::particle &c, cal::particle &d,
        cal::particle &e, cal::particle &f, cal::particle &g, cal::particle &h )
    {
    _particles[0] = &a;
    _particles[1] = &b;
    _particles[2] = &c;
    _particles[3] = &d;
    _particles[4] = &e;
    _particles[5] = &f;
    _particles[6] = &g;
    _particles[7] = &h;
    }

void bBox::update( JFLOAT )
    {
    }
