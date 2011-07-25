#include "jLib.informer.h"
#include "error/error.h"

BEGIN_JLIB_NAMESPACE

informer::informer()
    {
    }

informer::informer( const informer &in ) : observers( in.observers )
    {
    FOREACH( observers, ob )
        {
        (*ob)->informers.push_back( this );
        }
    }
informer::~informer()
    {
    clearObservers();
    }

void informer::clearObservers()
    {
    while( observers.size() )
        {
        removeObserver( observers.front() );
        }
    }

void informer::addObserver( observer *obs )
    {
    for( int x=0; x<observers.size(); x++ )
        {
        if( observers[x] == obs )
            {
            return;
            }
        }
    observers.push_back( obs );
    obs->informers.push_back( this );
    }
void informer::removeObserver( observer *in )
    {
    for( int x=0; x<observers.size(); x++ )
        {
        if( observers.at(x) == in )
            {
            observers.erase( observers.begin() + x );
            }
        }
    for( int x=0; x<in->informers.size(); x++ )
        {
        if( in->informers.at(x) == this )
            {
            in->informers.erase( in->informers.begin() + x );
            }
        }
    }

END_JLIB_NAMESPACE
