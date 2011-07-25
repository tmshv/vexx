#include "XString"
#include "XScene.h"
#include "XAbstractRenderer.h"
#include "XAbstractTexture.h"
#include "XDoodad.h"
#include "XCamera.h"
#include "XDebug"
#include "XPrimitives.h"

#include "XAbstractGeometryCache.h"
#include "XAbstractShader.h"

#include "QMouseEvent"
#include "QApplication"
#include "QGLWidget"
#include "QTimer"
#include "cmath"

#include "typeinfo"

class XTestDoodad : public XDoodad
    {
public:
    virtual void initiate()
        {
        cache = renderer()->getGeometryCache();
        cache->setVertices( XList<XVector3D>() << XVector3D( 0, 0, 0 ) );
        cache->setPoints( XList<uint>() << 0 );
        }

    virtual void render()
        {
        renderer()->drawGeomtreyCache( cache );
        }
private:
    XAbstractGeometryCache *cache;
    };

class XTriDoodad : public XDoodad
    {
public:
    virtual void initiate()
        {
        shader = renderer()->getShader( "attribute vec2 tex;"
                                        "varying vec2 coo;\n"
                                        "void main(void)\n"
                                        "{\n"
                                        "   coo = tex;\n"
                                        "   gl_Position = ftransform();\n"
                                        "}",
                                        "varying vec2 coo;\n"
                                        "uniform sampler2D smpl;"
                                        "void main(void)\n"
                                        "{\n"
                                        "   gl_FragColor = texture2D( smpl, coo );\n"
                                        "}" );

        XTexture *texture = new XTexture( renderer(), QImage("/home/jorj/Desktop/009.jpg") );
        addChild( texture );

        shader->getVariable("smpl")->setValue( texture );

        cache = renderer()->getGeometryCache();

        cache->setVertices( XList<XVector3D>()
                            << XVector3D( 0, 0, 0 )
                            << XVector3D( 0, 1, 0 )
                            << XVector3D( 1, 1, 0 ) );

        cache->setTriangles( XList<uint>() <<  0 << 1 << 2 );

        cache->setAttribute( "tex", XList<XVector2D>()
                            << XVector2D( 1, 0 )
                            << XVector2D( 0, 1 )
                            << XVector2D( 0, 0 ) );
        }

    virtual void render()
        {
        static float angle = 0;
        angle += 0.1;

        renderer()->setShader( shader );
        renderer()->drawGeomtreyCache( cache );
        renderer()->setShader( 0 );
        }
private:
    XAbstractGeometryCache *cache;
    XAbstractShader *shader;
    };

class glWidget : public QGLWidget, public XObject
    {
public:
    QGLContext *initRenderer()
        {
        renderer = new XGLRenderer( true );
        return renderer->context();
        }

    glWidget() : QGLWidget( initRenderer() ), camera( 55, XVector3D( 0, 5, 10 ) ), scene( renderer, &camera ), _initMouse( true )
        {
        timer = new QTimer;
        connect( timer, SIGNAL(timeout()), this, SLOT(updateGL()) );
        timer->start( 40 );

        setMouseTracking( true );
        setCursor( Qt::BlankCursor );

        _move << 0 << 0 << 0;
        }
    void initializeGL()
        {
        renderer->intialise();
        XTestDoodad *test1( new XTestDoodad );
        scene.addDoodad( test1 );

        scene.addDoodad( new XLocator );

        test1->addChild( new XTestDoodad );
        XDoodad *test2( new XTriDoodad );
        test1->addChild( test2 );

        XTransform trans;
        trans.translate( 1, 2, 3 );

        test1->setAttribute( XDoodad::HasTransform, true );
        test1->setTransform( trans );

        test2->setAttribute( XDoodad::HasTransform, true );
        test2->setTransform( trans );
        }

    void resizeGL( int w, int h )
        {
        scene.setViewportSize( QSize( w, h ) );
        }

    void paintGL()
        {
        camera.track( 0.5 * _move[0], 0.5 * _move[1], 0.5 * _move[2] );

        scene.renderScene( );
        }

    void keyPressEvent( QKeyEvent *event )
        {
        if( event->key() == Qt::Key_Escape )
            {
            QApplication::quit();
            }
        if( event->key() == Qt::Key_W )
            {
            _move[2] = -1;
            }
        if( event->key() == Qt::Key_S )
            {
            _move[2] = 1;
            }
        if( event->key() == Qt::Key_A )
            {
            _move[0] = -1;
            }
        if( event->key() == Qt::Key_D )
            {
            _move[0] = 1;
            }
        if( event->key() == Qt::Key_Q )
            {
            _move[1] = -1;
            }
        if( event->key() == Qt::Key_E )
            {
            _move[1] = 1;
            }
        }

    void keyReleaseEvent( QKeyEvent *event )
        {
        if( event->key() == Qt::Key_W )
            {
            _move[2] = 0;
            }
        if( event->key() == Qt::Key_S )
            {
            _move[2] = 0;
            }
        if( event->key() == Qt::Key_A )
            {
            _move[0] = 0;
            }
        if( event->key() == Qt::Key_D )
            {
            _move[0] = 0;
            }
        if( event->key() == Qt::Key_Q )
            {
            _move[1] = 0;
            }
        if( event->key() == Qt::Key_E )
            {
            _move[1] = 0;
            }
        }

    void mouseMoveEvent( QMouseEvent * )
        {
        if( _initMouse )
            {
            _oldPos = QCursor::pos();
            _initMouse = false;
            grabMouse();
            }

        QPointF pan( QPointF( QCursor::pos() - _oldPos ) * 0.05 );

        camera.pan( pan.x(), -pan.y() );

        QCursor::setPos( mapToGlobal( rect().center() ) );

        _oldPos = QCursor::pos();
        }

private:
    XGLRenderer *renderer;
    XPerspectiveCamera camera;
    XScene scene;
    QTimer *timer;
    QPoint _oldPos;
    bool _initMouse;
    XList <int> _move;
    };

int main( int i, char **c )
    {
    QApplication app( i, c );

    glWidget widg;

    widg.show();
    app.exec();
    }
