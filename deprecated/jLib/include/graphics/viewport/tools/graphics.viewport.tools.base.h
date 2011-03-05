#ifndef JLIB_GRAPHICS_VIEWPORT_TOOLS_BASE_H
#define JLIB_GRAPHICS_VIEWPORT_TOOLS_BASE_H

#include "graphics/viewport/tools/graphics.viewport.tools.types.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.base.h"
#include "graphics/viewport/graphics.viewport.manipulatorManager.h"
#include "graphics/viewport/graphics.viewport.types.h"

BEGIN_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

class JLIB_EXPORT base
    {
public:
    base( );
    virtual ~base( );
    bool isEnabled( );
    void setEnabled( bool = TRUE );
    bool isReadOnly( );
    void setReadOnly( bool = TRUE );
    void setClickLine( shape::line );
    void setTransform( math::matrix4x4 );
    math::matrix4x4 getTransform( );
    JFLOAT canHandleClickLineAt( );
    virtual entity::entityD *getDrawHandle();
    void setScene( entity::entityG * );
    bool isExitButtonState( int, int );
    bool isEntryButtonState( int, int );
    void setCanChangeSpace( bool = TRUE );
    bool canChangeSpace( );
    void setTransformSpace( manipulators::base::spaceType );
    static void setLineTolerance( JFLOAT );
    static void setPointTolerance( JFLOAT );
    static void setManipulatorScale( JFLOAT );
    static void setActiveColour( math::colour4 );
    virtual void highlight();
    virtual void reset();
    virtual void calculateClickLineIntersection( );
protected:
    shape::line currentClickLine( );
    shape::line previousClickLine( );
    void setClickLineHandleDistance( JFLOAT );
    JFLOAT lineTolerance( );
    JFLOAT pointTolerance( );
    JFLOAT manipulatorScale( );
    math::colour4 getActiveColour( );
    JFLOAT distanceToCamera( math::triple );
    entity::entityG *scene( );
private:
    entity::entityG *_pass;
    shape::line _currentClickLine;
    shape::line _previousClickLine;
    bool _readonly;
    bool _enabled;
    bool _canChangeSpace;
    math::quaternion _localOrientation;
    bool _isGlobal;
    JFLOAT _clickLineHandleDistance;
    math::matrix4x4 _transform;
    static JFLOAT _lineCollTol;
    static JFLOAT _pointCollTol;
    static math::colour4 _activeColour;
    static JFLOAT _manipulatorScale;
    };

END_JLIB_GRAPHICS_VIEWPORT_TOOLS_NAMESPACE

#endif
