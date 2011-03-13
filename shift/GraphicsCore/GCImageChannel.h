#ifndef GCIMAGECHANNEL_H
#define GCIMAGECHANNEL_H

#include "GCGlobal.h"
#include "spropertycontainer.h"
#include "sbaseproperties.h"
#include "spropertydata.h"

class GCImageChannelData : public SProperty
{
    // registers type information for SPropertyInfo, including save / load functions
   S_PROPERTY(GCImageChannelData, SProperty, SaveFunction, LoadFunction, AssignFunction, 0)

public:
   // called by parent
   static void SaveFunction( const SProperty *, SPropertyData &, SPropertyData::Mode); // Mode = Binary / ASCII
   static void LoadFunction( SProperty *, const SPropertyData &, xuint32, SPropertyData::Mode, SLoader & );
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
