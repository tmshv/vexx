#ifndef GCIMAGE_H
#define GCIMAGE_H
#include "spropertycontainer.h"
#include "GCImageChannel.h"

class GRAPHICSCORE_EXPORT GCImage : public SPropertyContainer
{
public:
    GCImage();

    void WritePixel(int x, int y); // (col Colour)

    XList <GCImageChannel> channels;
};

#endif // GCIMAGE_H
