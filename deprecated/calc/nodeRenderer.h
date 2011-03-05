#ifndef GLSLRENDERER_H
#define GLSLRENDERER_H

#include "XProperty"
#include "QObject"
#include "jLib.h"

class nodeEntity;
class APropertyDevice;
class AProperty;
class XColour;

class nodeRenderer : public QObject
    {
    Q_OBJECT
    XROProperty( nodeEntity *, node );
    XROProperty( QString, currentSource );
    jROProperty( bool, shouldPreMultiply );
public:
    enum DisplayModeEnum { DisplayRed=1, DisplayGreen=2, DisplayBlue=4, DisplayAlpha=8, DisplayRGBA=DisplayRed|DisplayGreen|DisplayBlue|DisplayAlpha };

    nodeRenderer( );
    ~nodeRenderer( );

    void setNode( nodeEntity * );
    void setTime( int );

    void render();
    jLib::graphics::pixel::texture &texture( );
    QRectF rect() const;

    XColour sampleColourAt( QPoint );

    void setGlobalMultiplier( double );
    void setRenderMode( DisplayModeEnum );
    void setPreMultiply( bool );

signals:
    void needsUpdate();
    void aspectChanged();

private slots:
    void changed( APropertyDevice *, AProperty * );
    void onConnection( APropertyDevice*, AProperty* );
    void childNeedsRebuild();
    void childNeedsUpdate();

private:
    bool _update;
    bool _rebuild;
    void rebuild();
    QString displayModeFormat( QString, QString ) const;

    QString sourceCode( ) const;

    jLib::graphics::shader::sFloat _multiplier;
    jLib::graphics::shader::variableSet _set;
    jLib::graphics::render::frameBuffer *_buffer;
    jLib::graphics::shader::surfaceShader *_shader;
    QRectF _imageRect;
    QObjectList _network;

    int _frame;

    bool _settingTime;
    DisplayModeEnum _renderMode;
    };

#ifdef SYNAPSE_RENDER_DEBUG
namespace renderDebug {
QList <const nodeRenderer *> renderers();
}
#endif

#endif // GLSLRENDERER_H
