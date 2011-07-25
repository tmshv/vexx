#ifndef XPRIMITIVES_H
#define XPRIMITIVES_H

#include "XDoodad.h"

class XAbstractGeometryCache;

class XLocator : public XDoodad
    {
public:
    ~XLocator();
    virtual void initiate();
    virtual void render();

private:
    XAbstractGeometryCache *cache;
    };

#endif // XPRIMITIVES_H
