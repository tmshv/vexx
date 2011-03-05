#include "GCImageChannel.h"

S_PROPERTY_CONTAINER_DEFINITION(GCImageChannel, SPropertyContainer) // registers these properties in the database
  S_PROPERTY_DEFINITION(UnsignedIntProperty, width)
  S_PROPERTY_DEFINITION(UnsignedIntProperty, height)
  S_PROPERTY_DEFINITION(GCImageChannelData, channelData)
S_PROPERTY_CONTAINER_END_DEFINITION(GCImageChannel, SPropertyContainer, saveContainer, loadContainer)

GCImageChannel::GCImageChannel()
{
}

void GCImageChannelData::SaveFunction( const SProperty* p_in, SPropertyData& data_in, SPropertyData::Mode mode_in)
{
    SProperty::save(p_in, data_in, mode_in); // saves the data of the parent class (keeps connections)

    const GCImageChannel* ptr = p_in->castTo<const GCImageChannel>(); // cast the input property to a GCImageChannel or 'name' (?)
    xAssert(ptr);
    if(ptr)
    {
        QByteArray arr;
        if(mode_in == SPropertyData::Binary) // Binary
        {
            QDataStream str(&arr, QIODevice::WriteOnly); // open a writeonly qstream, writes into arr
//            str << ptr->_value;
        }
        else // ASCII
        {
            QTextStream str(&arr, QIODevice::WriteOnly); // writes ascii into arr
//            str << ptr->_value;
        }
        data_in.setValue(arr); // set the data (this is written to disk later)
    }
}

void GCImageChannelData::LoadFunction( SProperty *, const SPropertyData &, xuint32, SPropertyData::Mode, SLoader & )
{
}

void GCImageChannelData::AssignFunction( const SProperty *, SProperty * )
{
}
