#ifndef XTRANSFORMEVENT_H
#define XTRANSFORMEVENT_H

#include "XProperty"
#include "XTransform.h"

class XDoodad;

class XTransformEvent
    {
public:
    XROProperty( XDoodad *, object );
    XROProperty( XTransform, oldTransform );

public:
    XTransformEvent( XDoodad *doodad, XTransform old );
    };

#endif // XTRANSFORMEVENT_H
