#ifndef JLIB_PTR_H
#define JLIB_PTR_H

#include "jLib.global.h"

BEGIN_JLIB_NAMESPACE

template <typename T> class JLIB_EXPORT jptr
    {
private:
    T *reference;
    bool silentRelease;
public:
    jptr( T *in=0 ) : silentRelease( FALSE )
        { reference = in; }
    /*jptr( T &in )
        { jFixme<<"This method isnt implemented becuase I couldnt replicate a caller for it."; }*/
    jptr( jptr &in ) : reference( in.reference ), silentRelease( in.silentRelease )
        { in.silentRelease = TRUE; }
    jptr( const jptr <T> &in ) : reference( in.reference ), silentRelease( FALSE )
        {;}
    ~jptr( )
        { release( ); }

    T* get()
        { return reference; }
    T &operator*()
        { return *reference; }
    T *operator->()
        { return reference; }
    void release( )
        {
        if( !silentRelease && reference != 0 )
            {
            reference = 0;
            delete reference;
            }
        }
    };
template <typename T> class JLIB_EXPORT jAptr
    {
private:
    T *reference;
    bool silentRelease;
public:
    jAptr( T *in=0 ) : silentRelease( FALSE )
        { reference = in; }
    /*jAptr( T &in )
        { jFixme<<"This method isnt implemented becuase i couldnt replicate a caller for it."; }*/
    jAptr( jAptr &in ) : reference( in.reference ), silentRelease( in.silentRelease )
        { in.silentRelease = TRUE; }
    jAptr( const jAptr <T> &in ) : reference( in.reference ), silentRelease( FALSE )
        {;}
    ~jAptr( )
        { release( ); }

    T* get()
        { return reference; }
    T &operator*()
        { return *reference; }
    T *operator->()
        { return reference; }
    T &operator[]( unsigned int x )
        { return reference[x]; }
    void release( )
        {
        if( !silentRelease && reference != 0 )
            {
            reference = 0;
            delete [] reference;
            }
        }
    };

END_JLIB_NAMESPACE

#endif
