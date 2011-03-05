#include "jLib.glHeader.h"
#include "nodeRenderer.h"
#include "nodeEntity.h"
#include "XColour"
#include "XDebug"
#include "QTime"

#ifdef SYNAPSE_RENDER_DEBUG
namespace renderDebug {
QList <const nodeRenderer *> _renderers;
QList <const nodeRenderer *> renderers()
    {
    return _renderers;
    }
}
#endif

inline QString rendererVertexShader( QRectF rect )
    {
    // create a vertex shader with the correct offsets
    return "void main()"                                                                                                            \
        "{"                                                                                                                         \
        "gl_TexCoord[0] = ( ( 0.5 * ( 1 + gl_Vertex ) ) * vec4( " + QString::number(rect.width()) + ", " + QString::number(rect.height()) + ", 0.0, 0.0 ) ) + vec4( " + QString::number(rect.left()) + ", " + QString::number(rect.top()) + ", 0.0, 0.0 );"  \
        "gl_Position = gl_Vertex;"                                                                                                  \
        "}";
    }

// and a default black fragment shader
#define POSTPROCESSFRAGSHADER "void main()"                     \
"{"                                                             \
"gl_FragColor = vec4( 0.0 );"                                   \
"}"

nodeRenderer::nodeRenderer( ) : _update( false ), _rebuild( false ), _multiplier( "PRI_GLOBAL_MULT" )
    {
    // initial multiplier is 1.0
    _multiplier.set( 1.0 );

    // init everything to 0, create them when we need them later ( context will be active )
    _buffer = 0;
    _shader = 0;
    XGetPropertyMember( node ) = 0;

    // frame is 0 by defualt too.
    _frame = 0;

    _settingTime = FALSE;

    _renderMode = DisplayRGBA;

#ifdef SYNAPSE_RENDER_DEBUG
    renderDebug::_renderers << this;
#endif
    }


nodeRenderer::~nodeRenderer( )
    {
#ifdef SYNAPSE_RENDER_DEBUG
    renderDebug::_renderers.removeAll( this );
#endif
    }

void nodeRenderer::childNeedsRebuild()
    {
    _update = TRUE;
    _rebuild = TRUE;
    emit needsUpdate();
    }

void nodeRenderer::childNeedsUpdate()
    {
    _update = TRUE;
    emit needsUpdate();
    }

void nodeRenderer::setNode( nodeEntity *n )
    {
    // set node
    XGetPropertyMember( node ) = n;

    // disconnect all old connections
    foreach( QObject *obj, _network )
        {
        disconnect( obj, SIGNAL(needsRebuild()), this, SLOT(childNeedsRebuild()) );
        disconnect( obj, SIGNAL(needsUpdate()), this, SLOT(childNeedsUpdate()) );
        disconnect( obj, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(changed(APropertyDevice*,AProperty*)) );
        disconnect( obj, SIGNAL(onConnection(APropertyDevice*,AProperty*)), this, SLOT(onConnection(APropertyDevice*,AProperty*)) );
        disconnect( obj, SIGNAL(onDisconnection(APropertyDevice*,AProperty*)), this, SLOT(onConnection(APropertyDevice*,AProperty*)) );
        }
    _network.clear();

    // if it isnt 0,
    if( n )
        {
        n->setExportRenderer( 0 );

        // find new connections.
        nodeEntityList nList( n->renderDependantNodeNetwork() );
        foreach( nodeEntity *informerNode, nList )
            {
            connect( informerNode, SIGNAL(needsRebuild()), this, SLOT(childNeedsRebuild()), Qt::UniqueConnection );
            connect( informerNode, SIGNAL(needsUpdate()), this, SLOT(childNeedsUpdate()), Qt::UniqueConnection );
            connect( informerNode, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(changed(APropertyDevice*,AProperty*)), Qt::UniqueConnection );
            connect( informerNode, SIGNAL(onConnection(APropertyDevice*,AProperty*)), this, SLOT(onConnection(APropertyDevice*,AProperty*)), Qt::UniqueConnection );
            connect( informerNode, SIGNAL(onDisconnection(APropertyDevice*,AProperty*)), this, SLOT(onConnection(APropertyDevice*,AProperty*)), Qt::UniqueConnection );
            _network << informerNode;
            }
        // setup the export renderer.
        n->setExportRenderer( this );

        // set the time of the new node.
        n->setTime( _frame );
        }

    // update everything
    _update = TRUE;
    _rebuild = TRUE;
    emit needsUpdate();
    }

void nodeRenderer::setTime( int frame )
    {
    // set the time up
    _settingTime = TRUE;
    _frame = frame;

    if( node() )
        {
        node()->setTime( frame );
        }
    _settingTime = FALSE;

    // well need an update too.
    _update = TRUE;
    emit needsUpdate();
    }

void nodeRenderer::onConnection( APropertyDevice*, AProperty* )
    {
    setNode( node() );
    }

jLib::graphics::pixel::texture &nodeRenderer::texture( )
    {
    // render if we need to
    render() GLE;
    return _buffer->texture( jLib::graphics::render::frameBuffer::COLOUR );
    }

void nodeRenderer::render( )
    {
    // if we dont have a framebuffer
    if( _buffer == 0 )
        {
        // we should probably allow the user to choose the fb quality... but not atm.

        // try a 16 bit floating point
        _buffer = new jLib::graphics::render::frameBuffer( jLib::graphics::render::frameBuffer::COLOUR|jLib::graphics::render::frameBuffer::DEPTH,
                                                           jLib::graphics::render::frameBuffer::COLOUR_64BIT_FLOAT );
        _buffer->setSize( 1, 1 );

        // if that failed, then try an 8 bit one
        if( !_buffer->isValid() )
            {
            _buffer = new jLib::graphics::render::frameBuffer( );
            _buffer->setSize( 1, 1 );

            if( !_buffer->isValid() )
                {
                // we cant really render on this graphics system...
                qDebug() << "Unsupported Graphics Card";
                abort();
                }
            }
        _buffer->setSize( 1, 1 );
        }

    // if we shoudl rebuild or update
    if( _update || _rebuild )
        {
        // tell our connected nodes to prepare for render
        if( node() )
            {
            nodeEntityList nList( node()->renderDependantNodeNetwork() );
            foreach( nodeEntity *informerNode, nList )
                {
                informerNode->prepareToRender();
                }
            }

        // rebuild first
        rebuild();

        // use buffer
        _buffer->start() GLE;

        // setup viewport
        glViewport( 0, 0, _imageRect.width(), _imageRect.height() ) GLE;
        glMatrixMode( GL_PROJECTION ) GLE;
        glLoadIdentity() GLE;
        glMatrixMode( GL_MODELVIEW ) GLE;
        glLoadIdentity() GLE;
        glClearColor( 0.0, 0.0, 0.0, 0.0 ) GLE;
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) GLE;
        glColor4f( 0.0, 0.0, 0.0, 0.0 );

        // bind shader
        if( _shader->isValid() )
            {
            _shader->use() GLE;
            _set.setValues() GLE;
            }

        // draw a basic quad
        glBegin( GL_QUADS );
            {
            glVertex3f( 1.0, 1.0, 0.0 );
            glVertex3f( 1.0, -1.0, 0.0 );
            glVertex3f( -1.0, -1.0, 0.0 );
            glVertex3f( -1.0, 1.0, 0.0 );
            }
        glEnd();

        // unbind shader
        if( _shader->isValid() )
            {
            _set.unsetValues() GLE;
            _shader->clear() GLE;
            }

        // stop buffer
        _buffer->finish() GLE;

        _update = FALSE;
        }
    }

QRectF nodeRenderer::rect() const
    {
    return _imageRect;
    }

void nodeRenderer::rebuild()
    {
    // ensure shader
    if( !_shader )
        {
        _shader = new jLib::graphics::shader::surfaceShader( POSTPROCESSFRAGSHADER, rendererVertexShader( _imageRect ).toStdString() ) GLE;
        _set.setShader( *_shader );
        }
    // if we have a node
    if( node() )
        {
        // setup the render rectangle
        QRectF oldRect( _imageRect );
        _imageRect = node()->rect();
        if( _imageRect.width() <= 0 )
            {
            _imageRect.setWidth( 1 );
            }
        if( _imageRect.height() <= 0 )
            {
            _imageRect.setHeight( 1 );
            }
        if( _imageRect.width() > 5000 )
            {
            _imageRect.setWidth( 5000 );
            }
        if( _imageRect.height() > 5000 )
            {
            _imageRect.setHeight( 5000 );
            }
        // jiggle aspect if we need to
        if( _imageRect != oldRect )
            {
            emit aspectChanged();
            _rebuild = TRUE;
            }

        // if rebuilding
        if( _rebuild )
            {
            // setup size
            _buffer->setSize( _imageRect.width(), _imageRect.height() ) GLE;

            // ensure node list up to date before generating source
            _set.clear() GLE;
            nodeEntityList network( node()->renderDependantNodeNetwork() );
            foreach( nodeEntity *n, network )
                {
                n->addVariables( _set ) GLE;
                }
            _set.add( _multiplier );

            if( node() )
                {
                nodeEntityList nList( node()->renderDependantNodeNetwork() );
                foreach( nodeEntity *informerNode, nList )
                    {
                    informerNode->prepareToRebuild();
                    }
                }

            _currentSource = sourceCode();

            // set source of shader and rebuild
            _shader->setSource( _currentSource.toStdString(), rendererVertexShader( _imageRect ).toStdString() ) GLE;
            _shader->rebuild() GLE;

            // if we have a valid shader
            if( _shader->isValid() )
                {
                GLE;
                _shader->use() GLE;
                _shader->clear() GLE;
                // ensure vairables up to date
                _set.refreshLocations() GLE;

                _rebuild = FALSE;
                }
            else
                {
                // otherwise print debug stuff
                ALog << "The shader engine has encountered an error, debugging data has been printed to the terminal.";
                for( unsigned int x=0; x<_shader->sizeErrors(); x++ )
                    {
                    qDebug() << QString::fromStdString( _shader->getError( x ) );
                    }
                _shader->clearErrors();
                qDebug() << QString::fromStdString( _shader->getFragmentSource() ) << endl << QString::fromStdString( _shader->getVertexSource() );
                }
            }
        }
    else if( _rebuild )
        {
        // just rebuild to black
        _shader->setSource( POSTPROCESSFRAGSHADER, rendererVertexShader( _imageRect ).toStdString() ) GLE;
        _shader->rebuild() GLE;
        _set.clear();
        _rebuild = FALSE;
        _imageRect = QRect();
        }
    }

void nodeRenderer::changed( APropertyDevice *, AProperty *data )
    {
    if( data )
        {
        // update if an attribute changed, rebuild for evertyhing else
        if( data->attributes().contains("Attribute") )
            {
            _update = TRUE;
            if( _settingTime == FALSE )
                {
                emit needsUpdate();
                }
            }
        else
            {
            // rebuild if the source changed
            if( data->name() == "nodeEntity/source" )
                {
                _rebuild = TRUE;
                if( _settingTime == FALSE )
                    {
                    emit needsUpdate();
                    }
                }
            }
        }
    }

QString nodeRenderer::sourceCode( ) const
    {
    // if we have a node
    if( node() )
        {
        QHash <QString, QString> functions;

        nodeEntityList network( node()->renderDependantNodeNetwork() );
        foreach( nodeEntity *n, network )
            {
            // grab the trees definitions
            n->definitions( functions );
            }

        QString source;

        //add uniform variables
            {
            FOREACHCONST( _set, var )
                {
                source += QString::fromStdString( (*var)->glslUniformVariableDefinition() ) + ";\n";
                }
            }

        source += "\n";

        //Function definitions
            {
            FOREACH( functions, fn )
                {
                QString thisFn = "vec4 " + fn.key() + "( vec2 );\n";
                source += thisFn;
                }
            }

        source += "\n";

        //Function source
            {
            FOREACH( functions, fn )
                {
                QString thisFn = "vec4 " + fn.key() + "( vec2 coords )\n{\n" + fn.value() + "\n}\n\n";
                source += thisFn;
                }
            }

        return source + "void main() { vec2 coords;coords=gl_TexCoord[0].st;vec4 adjColour = vec4(" + node()->call( "( coords )" ) + ");adjColour *= vec4(PRI_GLOBAL_MULT);" + displayModeFormat( "gl_FragColor", "(adjColour)") + "; }";
        }
    return "void main() { gl_FragColor = 0.0; }";
    }

QString nodeRenderer::displayModeFormat( QString ret, QString in ) const
    {
    QString premult;
    if( _shouldPreMultiply )
        {
        premult = "vec4 preMult=vec4(" + in + ");preMult.rgb *= preMult.a;";
        in = "preMult";
        }

    // display correct channels
    if( _renderMode == DisplayRGBA )
        {
        return premult + ret + " = " + in;
        }
    else if( _renderMode == DisplayRed )
        {
        return premult + "vec4 tmp = vec4(" + in + ");" + ret + " = vec4( tmp.r, tmp.r, tmp.r, 1.0 );";
        }
    else if( _renderMode == DisplayGreen )
        {
        return premult + "vec4 tmp = vec4(" + in + ");" + ret + " = vec4( tmp.g, tmp.g, tmp.g, 1.0 );";
        }
    else if( _renderMode == DisplayBlue )
        {
        return premult + "vec4 tmp = vec4(" + in + ");" + ret + " = vec4( tmp.b, tmp.b, tmp.b, 1.0 );";
        }
    else if( _renderMode == DisplayAlpha )
        {
        return premult + "vec4 tmp = vec4(" + in + ");" + ret + " = vec4( tmp.a, tmp.a, tmp.a, 1.0 );";
        }
    else
        {
        return ret + " = vec4( 0.0 );";
        }
    }

XColour nodeRenderer::sampleColourAt( QPoint pt )
    {
    // sample the colour of the buffer at the point
    pt -= _imageRect.topLeft().toPoint();
    if( pt.x() >= 0 && pt.y() >= 0 && pt.x() < _imageRect.width() && pt.y() < _imageRect.height() )
        {
        return XColour( &_buffer->colourAt( pt.x(), pt.y() )[0] ) / _multiplier.get();
        }
    return XColour( 0.0, 0.0, 0.0, 0.0 );
    }

void nodeRenderer::setGlobalMultiplier( double in )
    {
    _multiplier.set( in );
    _update = true;
    emit needsUpdate();
    }

void nodeRenderer::setRenderMode( DisplayModeEnum en )
    {
    _renderMode = en;

    _update = true;
    _rebuild = true;
    emit needsUpdate();
    }

void nodeRenderer::setPreMultiply( bool premult )
    {
    _shouldPreMultiply = premult;

    _update = true;
    _rebuild = true;
    emit needsUpdate();
    }
