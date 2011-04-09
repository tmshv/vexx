#include "sarrayproperty.h"


template <typename T> void SArrayProperty<T>::SaveFunction( const SProperty* p_in, SPropertyData& data_in, SPropertyData::Mode mode_in) // Mode = Binary / ASCII
{
    SProperty::save(p_in, data_in, mode_in); // saves the data of the parent class (keeps connections)

    const SArrayProperty* ptr = p_in->castTo<SArrayProperty>(); // cast the input property to an SArrayProperty
    xAssert(ptr);
    if(ptr)
    {
        QByteArray arr;
        if(mode_in == SPropertyData::Binary) // Binary
        {
            QDataStream str(&arr, QIODevice::WriteOnly); // open a writeonly qstream, writes into arr
            str << ptr->mData;
        }
        else // ASCII
        {
            QTextStream str(&arr, QIODevice::WriteOnly); // writes ascii into arr
            const Eigen::Array <T, Eigen::Dynamic, Eigen::Dynamic> &data = ptr->mData;
            str << data;
        }
        data_in.setValue(arr); // set the data (this is written to disk later)
    }
}

template <typename T> void SArrayProperty<T>::LoadFunction( SProperty* p_in, const SPropertyData& data_in, xuint32 v_in, SPropertyData::Mode mode_in, SLoader &l_in)
{

    SProperty::load(p_in, data_in, v_in, mode_in, l_in);

    SArrayProperty* ptr = p_in->castTo<SArrayProperty>();
    xAssert(ptr);
    if(ptr)
    {
        QByteArray arr = data_in.value();
        if(mode_in == SPropertyData::Binary)
        {
            QDataStream str(&arr, QIODevice::ReadOnly);
            str >> ptr->mData;
        }
        else
        {
            QTextStream str(&arr, QIODevice::ReadOnly);
            str >> ptr->mData;
        }
    }
}

