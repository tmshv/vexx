#include "XDebug"
#include "timeNow.h"
#include "TestClasses.h"
#include <iostream>

#define TESTCYCLES 1000000

int main( )
  {
    {
    uint16_t i[1024];
    uint16_t j[1024];
    uint16_t k[1024];

    for(int x=0; x<1024; ++x)
      {
      k[x] = i[x] * j[x];
      }

    for(int x=0; x<1024; ++x)
      {
      std::cout << k[x] << std::endl;
      }
    }

  return EXIT_SUCCESS;
}

#if 0
    XObjectList list;
    QObjectList list2;
    list << new TestA << new TestB << new TestC;
    list2 << new TestA << new TestB << new TestC;

    int num1( 0 );
    int num2( 0 );
    int num3( 0 );

    double begin1( timeNow() );

    for( int x=0; x<TESTCYCLES; x++ )
        {
        foreach( QObject *obj, list2 )
            {
            if( qobject_cast<QObject*>(obj) )
                {
                num1++;
                }
            if( qobject_cast<TestA*>(obj) )
                {
                num1 += qobject_cast<TestA*>(obj)->fn();
                }
            if( qobject_cast<TestB*>(obj) )
                {
                num1 += qobject_cast<TestB*>(obj)->fn();
                }
            if( qobject_cast<TestC*>(obj) )
                {
                num1 += qobject_cast<TestC*>(obj)->fn();
                }
            }
        }

    double end1( timeNow() );
    xDebug << "33% Complete";
    double begin2( timeNow() );

    for( int x=0; x<TESTCYCLES; x++ )
        {
        foreach( XObject *obj, list )
            {
            if( xobject_cast<XObject*>(obj) )
                {
                num2++;
                }
            if( xobject_cast<TestA*>(obj) )
                {
                num2 += xobject_cast<TestA*>(obj)->fn();
                }
            if( xobject_cast<TestB*>(obj) )
                {
                num2 += xobject_cast<TestB*>(obj)->fn();
                }
            if( xobject_cast<TestC*>(obj) )
                {
                num2 += xobject_cast<TestC*>(obj)->fn();
                }
            }
        }

    double end2( timeNow() );
    xDebug << "66% Complete";
    double begin3( timeNow() );

    for( int x=0; x<TESTCYCLES; x++ )
        {
        foreach( XObject *obj, list )
            {
            if( dynamic_cast<XObject*>(obj) )
                {
                num3++;
                }
            if( dynamic_cast<TestA*>(obj) )
                {
                num3 += xobject_cast<TestA*>(obj)->fn();
                }
            if( dynamic_cast<TestB*>(obj) )
                {
                num3 += xobject_cast<TestB*>(obj)->fn();
                }
            if( dynamic_cast<TestC*>(obj) )
                {
                num3 += xobject_cast<TestC*>(obj)->fn();
                }
            }
        }

    double end3( timeNow() );

    xDebug << "Tests Passed:" << ((num1 == num2 && num2 == num3) ? "Yes" : "No") << endl
           << "QObject Cast took:" << (end1-begin1) << endl
           << "XObject Cast took:" << (end2-begin2) << endl
           << "Dyanmic Cast took:" << (end3-begin3) << endl
           << "XObject Cast was" <<  (end3-begin3) / (end2-begin2) << "times faster than Dynamic Cast" << endl
           << "and" << (end1-begin1) / (end2-begin2) << "faster than QObject Cast";
    }

#endif
