#ifndef JLIB_GRAPHICS_VIEWMANIPULATOR_H
#define JLIB_GRAPHICS_VIEWMANIPULATOR_H

#include "graphics/viewport/graphics.viewport.types.h"
#include "graphics/primitives/graphics.primitives.pointCloud.h"
#include "graphics/render/graphics.render.scene.h"
#include "graphics/viewport/manipulators/graphics.viewport.manipulators.types.h"
#include "graphics/viewport/tools/graphics.viewport.tools.types.h"
#include <sys/time.h>

#ifdef QT_CORE_LIB
#include <Qt>

#define JLIB_QT_MANIPULATOR_WIDGET public: \
jLib::graphics::viewport::manipulatorManager manipulators; \
void mouseMoveEvent( QMouseEvent *event ) \
{ manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 ); } \
void mousePressEvent( QMouseEvent *event ) \
    { manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 ); } \
void mouseReleaseEvent( QMouseEvent *event ) \
    { manipulators.mouseEvent(event->x(), event->y(), event->buttons(), ( event->modifiers() & Qt::ShiftModifier ) != 0, ( event->modifiers() & Qt::ControlModifier ) != 0, ( event->modifiers() & Qt::AltModifier ) != 0 ); } \
void keyPressEvent( QKeyEvent *event ) \
    { manipulators.keyEvent( event->key(), TRUE, event->isAutoRepeat() ); } \
void keyReleaseEvent( QKeyEvent *event ) \
    { manipulators.keyEvent( event->key(), FALSE, event->isAutoRepeat() ); } \
void wheelEvent( QWheelEvent *event ) \
    { manipulators.scrollEvent( event->delta() ); } \

#else
#define JLIB_QT_MANIPULATOR_WIDGET
#endif

BEGIN_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

struct mouseEvent
    {
    unsigned int mouseX;
    unsigned int mouseY;
    int buttons;
    int modifiers;
    time_t timeSeconds;
    unsigned int timeMilliseconds;
    };

class JLIB_EXPORT manipulatorManager
    {
public:
    struct entityRef
        {
        enum entityType { point };
        entityType type;
        math::matrix4x4 local;
        bool illustrate;
        bool selected;
        template <typename T> T *getAs()
            {
            return reinterpret_cast <math::triple *> (entity);
            }
    private:
        void *entity;
        friend class manipulatorManager;
        };
    enum keyCodes
        {
#ifdef Qt_CORE_LIB
        KSHIFT = Qt::Key_Shift,
        KBACKSLASH = Qt::Key_Backslash,
        KSPACE = Qt::Key_Space
#else
        KSHIFT,
        KBACKSLASH,
        KSPACE
#endif
        };
    enum mouseButtons { LEFT = 1, RIGHT = 2, MIDDLE = 4 };
    enum mouseModifiers { MSHIFT = 1, MCTRL = 2, MALT = 4 };
    enum toolReturn { ACTIVE = 1, PASSIVE = 2, UPDATE = 4 };
    math::colour4 dormantColour;
    math::colour4 activeColour;

    manipulatorManager(  );
    manipulatorManager( render::scene & );
    void mouseEvent( unsigned int, unsigned int, int, bool, bool, bool);
    void keyEvent( int, bool, bool );
    void scrollEvent( int );
    void addHandler( manipulators::base * );
    void addEntity( math::triple &, math::matrix4x4 local=math::matrix4x4() );
    void removeHandler( manipulators::base * );
    bool needsUpdate();
    void updated();
    vector <entityRef *> getEntitys( entityRef::entityType );
    vector <entityRef *> getSelectedEntitys( entityRef::entityType );
    entityRef *getEntity( math::triple & );
    void deselectAll();
    void rebuildDrawLayer();
    bool getChanged();
    void setChanged( bool = true );
    void setUpdate( bool = true );
    void activate( manipulators::base & );
    void deactivate( );
    void setScene( render::scene & );

protected:
    void handleCurrentManip();
    void handleCurrentTool();
    struct mouseEvent currentMouse;
    struct mouseEvent oldMouse;
    render::scene *_parentView;
    vector <manipulators::base *> handlers;
    bool _update;

    void distributeEventToHandlers();
private:
    vector <entityRef> _entities;
    //primitives::pointCloud _points;
    bool _changed;
    manipulators::base *_currentActiveManip;
    };

END_JLIB_GRAPHICS_VIEWPORT_NAMESPACE

#endif
