#ifndef NODEENTITYMANIPULATOR_H
#define NODEENTITYMANIPULATOR_H

#include "QPointF"
#include "QSizeF"
#include "nodeEntity.h"
#include "XProperty"
#include "synapseSDK.h"

class QPainter;

class SYNPASE_SDK_EXPORT nodeEntityManipulator
    {
public:
    enum DrawMode { Full, Minimal };
    XProperty( DrawMode, drawMode, setDrawMode );

    enum ManipulatorMode { Translate=1, Rotate=2, Scale=3 };
    XProperty( ManipulatorMode, mode, setMode );

    enum UsableRegions { TranslateGeneral, TranslateX, TranslateY, RotateZ, ScaleGeneral, ScaleX, ScaleY };
    XProperty( UsableRegions, useRegion, setUseRegion );

    XProperty( QPointF, clickOffset, setClickOffset );

    XProperty( QPointF, position, setPosition );
    XProperty( qreal, angle, setAngle );
    XProperty( QPointF, scale, setScale );
    XProperty( QPointF, viewScale, setViewScale );

public:
    nodeEntityManipulator( QPointF pos=QPointF(0,0), DrawMode=Full );
    nodeEntityManipulator( qreal scaX, qreal scaY, DrawMode=Full );
    nodeEntityManipulator( qreal ang, DrawMode=Full );

    void paint( QPainter *pai );
    void setViewScale( qreal, qreal );

    bool canUse( QPointF pos );
    void use( QPointF now, QPointF previous );
    };

#endif // NODEENTITYMANIPULATOR_H
