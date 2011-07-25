#ifndef TRANSFORMNODE_H
#define TRANSFORMNODE_H

#include "nodeEntity.h"

class transformNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( transformNode, "psynapse.placid-acid.com", "Transform/Transform", QColor( 164, 41, 43 ) );
public:
    transformNode();

    virtual QRectF rect();

public slots:
    virtual void updateTransform(APropertyDevice*,AProperty*);
    };

class scaleNode : public transformNode
    {
    Q_OBJECT
    NODE_TYPE( scaleNode, "psynapse.placid-acid.com", "Transform/Scale", QColor( 144, 44, 42.0 ) );
public:
    scaleNode();

    virtual void manipulatorChanged( int );
    virtual void initiateManipulators( );

    virtual void updateTransform(APropertyDevice*,AProperty*);
    };

class translateNode : public transformNode
    {
    Q_OBJECT
    NODE_TYPE( translateNode, "psynapse.placid-acid.com", "Transform/Translate", QColor( 182, 57, 53 ) );
public:
    translateNode();

    virtual void manipulatorChanged( int );
    virtual void initiateManipulators( );

    virtual void updateTransform(APropertyDevice*,AProperty*);
    };

class rotateNode : public transformNode
    {
    Q_OBJECT
    NODE_TYPE( rotateNode, "psynapse.placid-acid.com", "Transform/Rotate", QColor( 164, 63, 79 ) );
public:
    rotateNode();

    virtual void manipulatorChanged( int );
    virtual void initiateManipulators( );

    virtual void updateTransform(APropertyDevice*,AProperty*);
    };

#endif // TRANSFORMNODE_H
