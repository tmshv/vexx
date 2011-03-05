#ifndef JLIB_GRAPHICS_GLQTWIDGET_H
#define JLIB_GRAPHICS_GLQTWIDGET_H

#include "jqt/jqt.types.h"

#ifdef QT_OPENGL_LIB

#include "QtOpenGL/qgl.h"
#include <QGLWidget>
#include <QGLFormat>

// Not using macros because moc cant handle them
namespace jLib
{
namespace jqt
{

class JLIB_EXPORT widget : public QGLWidget
    {
    Q_OBJECT

public:
    //graphics::viewport::renderPass engine;
    //graphics::render::scene rScene;
    graphics::render::architecture architecture;
    graphics::render::scene root;
    widget( QWidget *parent=0, math::colour4 colour=math::colour4( 0.75, 0.75, 0.75 ), bool multiSample=FALSE );

protected:
    void initializeGL();
    virtual void setupGL();
    void resizeGL( int _w, int _h );
    void paintGL();

private:
    QGLFormat getFormat( bool );
    };

}
}

#endif

#endif
