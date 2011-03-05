#ifndef JLIB_INFORMER_H
#define JLIB_INFORMER_H

#include "jLib.global.h"
#include "QList"

BEGIN_JLIB_NAMESPACE

class observer;

class JLIB_EXPORT informer
    {
public:
    informer();
    informer( const informer &in );
    virtual ~informer();
    void clearObservers();

protected:
    void addObserver( observer * );
    void removeObserver( observer * );
    QList <observer *> observers;
    friend class observer;
    };

class JLIB_EXPORT observer
    {
public:
    observer()
        {
        }
    observer( const observer &in ) : informers( in.informers )
        {
        FOREACH( informers, inf )
            {
            (*inf)->observers.push_back( this );
            }
        }
    virtual ~observer()
        {
        clearInformers();
        }

protected:
    void clearInformers()
        {
        while( informers.size() )
            {
            informers.front()->removeObserver( this );
            }
        }

private:
    QList <informer *> informers;
    friend class informer;
    };

END_JLIB_NAMESPACE

#define jLibInformerObserverPairNoArguments( _EXPORT, name, fn, inf ) \
class _EXPORT name : public jLib::observer                          \
    {                                                               \
public:                                                             \
    virtual void fn() = 0;                                          \
    };                                                              \
class _EXPORT inf : public jLib::informer                           \
    {                                                               \
public:                                                             \
    void addObserver( name *obs )                                   \
        {                                                           \
        informer::addObserver( obs );                               \
        }                                                           \
    void removeObserver( name *obs )                                \
        {                                                           \
        informer::removeObserver( obs );                            \
        }                                                           \
    void fn( )                                                      \
        {                                                           \
        for( int x=0; x<observers.size(); x++ )                     \
            {                                                       \
            static_cast <name *> (observers[x])->fn();              \
            }                                                       \
        }                                                           \
    };

#define jLibInformerObserverPairOneArgument( _EXPORT, name, fn, inf, arg )   \
class _EXPORT name : public jLib::observer                          \
    {                                                               \
public:                                                             \
    virtual void fn( arg ) = 0;                                     \
    };                                                              \
class _EXPORT inf : public jLib::informer                               \
    {                                                               \
public:                                                             \
    void addObserver( name *obs )                                   \
        {                                                           \
        informer::addObserver( obs );                               \
        }                                                           \
    void removeObserver( name *obs )                                \
        {                                                           \
        informer::removeObserver( obs );                            \
        }                                                           \
    void fn( arg a )                                                \
        {                                                           \
        for( int x=0; x<observers.size(); x++ )                     \
            {                                                       \
            static_cast <name *> (observers[x])->fn( a );           \
            }                                                       \
        }                                                           \
    };

#define jLibInformerObserverPairTwoArguments( _EXPORT, name, fn, inf, arg, arg2 )  \
class _EXPORT name : public jLib::observer                          \
    {                                                               \
public:                                                             \
    virtual void fn( arg, arg2 ) = 0;                               \
    };                                                              \
class _EXPORT inf : public jLib::informer                           \
    {                                                               \
public:                                                             \
    void addObserver( name *obs )                                   \
        {                                                           \
        informer::addObserver( obs );                               \
        }                                                           \
    void removeObserver( name *obs )                                \
        {                                                           \
        informer::removeObserver( obs );                            \
        }                                                           \
    void fn( arg a, arg2 b )                                        \
        {                                                           \
        for( int x=0; x<observers.size(); x++ )                     \
            {                                                       \
            static_cast <name *> (observers[x])->fn( a, b );        \
            }                                                       \
        }                                                           \
    };

#endif
