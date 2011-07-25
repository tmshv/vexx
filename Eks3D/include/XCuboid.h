#ifndef XCUBOID_H
#define XCUBOID_H

#include "X3DGlobal.h"
#include "XVector3D"
#include "XSize"
#include "XProperty"

class EKS3D_EXPORT XCuboid
    {
public:
    XROProperty( XVector3D, minimum );
    XROProperty( XVector3D, maximum );
    XROProperty( bool, isValid )

public:
    XCuboid( );
    XCuboid( XVector3D minimum, XVector3D maximum );
    XCuboid( XVector3D minimum, XSize );

    XCuboid united( const XVector3D & ) const;
    XCuboid united( const XCuboid & ) const;

    XCuboid &operator|=( const XCuboid & );
    XCuboid operator|( const XCuboid & ) const;

    void setMinimum( const XVector3D & );
    void setMaximum( const XVector3D & );

    XSize size() const;

    bool isInside( const XVector3D & ) const;
    bool isInside( const XCuboid & ) const;
    };

#endif // XCUBOID_H
