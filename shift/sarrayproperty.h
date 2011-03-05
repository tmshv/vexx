#ifndef SARRAYPROPERTY_H
#define SARRAYPROPERTY_H

#include "spropertycontainer.h"
#include "sbaseproperties.h"
#include "spropertydata.h"
#include "XList"

// reimplement stream for QTextStream to allow it to work with template classes

template <typename T> QTextStream & operator <<(QTextStream &str, const XList<T> &data)
{
    int size = data.size();
    str << size << " ";
    for (int i = 0; i < size; ++i)
    {
        str << data[i];
        if(i < size-1) // while not last element
        {
            str << " "; // separate each element with space
        }
    }
    return str;
}

template <typename T> QTextStream & operator >>(QTextStream &str, XList<T> &data)
{
    xuint32 size;
    str >> size; // first element in str is size of str
    for(int i = 0; i < size; ++i )
    {
        T tVal;
        str >> tVal;
        data.append(tVal);
    }
    return str;
}



template <typename T> class SArrayProperty : public SProperty
{
    S_PROPERTY(SArrayProperty, SProperty, SaveFunction, LoadFunction, AssignFunction)
public:

    // called by parent
    static void SaveFunction( const SProperty* p_in, SPropertyData& data_in, SPropertyData::Mode mode_in); // Mode = Binary / ASCII
    static void LoadFunction( SProperty* p_in, const SPropertyData& data_in, xuint32 v_in, SPropertyData::Mode mode_in, SLoader&); // Mode = Binary / ASCII
    static void AssignFunction( const SProperty *, SProperty * ) {} // typecheck this when implementing
#if 0
    void WriteData(int index, T value)
    {
        mData.insert(index, value);
    }
#endif

private:
    XList <T> mData;
};




class SFloatArrayProperty : public SArrayProperty<float>
{
   S_PROPERTY(SFloatArrayProperty, SProperty, SaveFunction, LoadFunction, AssignFunction)
public:


};


#endif // SARRAYPROPERTY_H
