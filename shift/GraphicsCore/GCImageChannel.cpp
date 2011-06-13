#include "GCImageChannel.h"

S_PROPERTY_CONTAINER_DEFINITION(GCImageChannel, SPropertyContainer) // registers these properties in the database
  S_PROPERTY_DEFINITION(UnsignedIntProperty, width, 0)
  S_PROPERTY_DEFINITION(UnsignedIntProperty, height, 0)
  S_PROPERTY_DEFINITION(GCImageChannelData, channelData)
S_PROPERTY_CONTAINER_END_DEFINITION(GCImageChannel, SPropertyContainer, saveContainer, loadContainer)

GCImageChannel::GCImageChannel()
{
}

void GCImageChannelData::SaveFunction( const SProperty* p_in, SSaver &s)
{
    SProperty::save(p_in, s); // saves the data of the parent class (keeps connections)
    //writeValue(s, ptr->_value);
}

SProperty *GCImageChannelData::LoadFunction( SPropertyContainer *p_in, SLoader &l )
  {
  return SProperty::load(p_in, l);
  // readValue(l, prop->_value);
  }

void GCImageChannelData::AssignFunction( const SProperty *, SProperty * )
{
}
