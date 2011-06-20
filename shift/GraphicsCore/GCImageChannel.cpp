#include "GCImageChannel.h"

S_PROPERTY_CONTAINER_DEFINITION(GCImageChannel) // registers these properties in the database
  S_PROPERTY_DEFINITION(UnsignedIntProperty, width, 0)
  S_PROPERTY_DEFINITION(UnsignedIntProperty, height, 0)
  S_PROPERTY_DEFINITION(GCImageChannelData, channelData)
S_PROPERTY_CONTAINER_END_DEFINITION(GCImageChannel)

GCImageChannel::GCImageChannel()
{
}

void GCImageChannelData::saveProperty( const SProperty* p_in, SSaver &s)
{
    SProperty::saveProperty(p_in, s); // saves the data of the parent class (keeps connections)
    //writeValue(s, ptr->_value);
}

SProperty *GCImageChannelData::loadProperty( SPropertyContainer *p_in, SLoader &l )
  {
  return SProperty::loadProperty(p_in, l);
  // readValue(l, prop->_value);
  }

void GCImageChannelData::assignProperty( const SProperty *, SProperty * )
{
  xAssertFail();
}
