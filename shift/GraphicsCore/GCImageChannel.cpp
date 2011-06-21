#include "GCImageChannel.h"
#include "styperegistry.h"

S_IMPLEMENT_PROPERTY(GCImageChannelData)

const SPropertyInformation *GCImageChannelData::createTypeInformation()
  {
  return SPropertyInformation::create<GCImageChannelData>("GCImageChannelData");
  }


S_IMPLEMENT_PROPERTY(GCImageChannel)

const SPropertyInformation *GCImageChannel::createTypeInformation()
  {
  SPropertyInformation *info = SPropertyInformation::create<GCImageChannel>("GCImageChannel");
  info->add(&GCImageChannel::width, "width");
  info->add(&GCImageChannel::height, "height");
  info->add(&GCImageChannel::channelData, "channelData");
  return info;
  }

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
