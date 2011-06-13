#ifndef GCIMAGECHANNEL_H
#define GCIMAGECHANNEL_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "sbaseproperties.h"

class GCImageChannelData : public SProperty
{
    // registers type information for SPropertyInfo, including save / load functions
   S_PROPERTY(GCImageChannelData, SProperty, SaveFunction, LoadFunction, AssignFunction, 0)

public:
   // called by parent
   static void SaveFunction( const SProperty *, SSaver &); // Mode = Binary / ASCII
   static SProperty *LoadFunction( SPropertyContainer *, SLoader & );
   static void AssignFunction( const SProperty *, SProperty * ); // typecheck this when implementing

   void WritePixel(int index, float value);

   XList <float> data; // array of data
};


class GCImageChannel : public SPropertyContainer
{
    // Registers property info
    S_PROPERTY_CONTAINER(GCImageChannel, SPropertyContainer, 0)

public:
    GCImageChannel();

    UnsignedIntProperty width;
    UnsignedIntProperty height;
    GCImageChannelData channelData;
};

#endif // GCIMAGECHANNEL_H
