#include "jLib.glHeader.h"
#include "viewer.h"
#include "viewportEntity.h"
#include "QResizeEvent"
#include "QWheelEvent"
#include "QContextMenuEvent"
#include "nodeRenderer.h"
#include "nodeEntityManipulator.h"
#include "QPainter"
#include "QMatrix4x4"
#include "QGLFormat"
#include "utils.h"

QGLFormat glFmt()
    {
    QGLFormat format;
    format.setSampleBuffers( true );
    return format;
    }

viewer::viewer( QWidget *parent, APlugin *a ) : QGLWidget( glFmt(), parent ), _frame( 0 )
    {
    _isHorizontallySplit = true;
    _isSplitEnabled = false;

    setAutoFillBackground( false );

    _activeNode = 0;
    _activeManipulator = 0;

    _app = a;

    // connect this to app signals
    connect( app(), SIGNAL(entityAdded(AEntity*)), this, SLOT(entityAdded(AEntity*)) );
    connect( app(), SIGNAL(entityRemoved(AEntity*)), this, SLOT(entityRemoved(AEntity*)) );
    connect( &_renderer, SIGNAL(needsUpdate()), this, SLOT(update()) );
    connect( &_renderer, SIGNAL(aspectChanged()), this, SLOT(updateViewTransform()) );
    connect( &_rendererSecondary, SIGNAL(needsUpdate()), this, SLOT(update()) );
    connect( &_rendererSecondary, SIGNAL(aspectChanged()), this, SLOT(updateViewTransform()) );

    connect( app(), SIGNAL(cleanState()), this, SLOT(clearViewer()) );
    connect( app(), SIGNAL(openState()), this, SLOT(clearViewer()) );

    connect( app(), SIGNAL(cleanState()), this, SLOT(refreshViewNodeFromArray()) );
    connect( app(), SIGNAL(openState()), this, SLOT(refreshViewNodeFromArray()) );
    connect( app(), SIGNAL(insertState()), this, SLOT(checkNodes()) );

    connect( app(), SIGNAL(selectionChanged()), this, SLOT(initiateManipulators()) );
    connect( app(), SIGNAL(selectionChanged()), this, SLOT(update()) );

    // we want to rebuild
    _rebuild = TRUE;

    // setup view matrix
    resetView();
    _transformNeedsUpdate = TRUE;

    _globalAdjustment.scale( 1000, 1000 );
    _globalAdjustment.translate( -0.5, -0.5 );

    _isSampling = false;

    setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    _activePickerMode = None;
    _activePicker = -1;
    }

viewer::~viewer()
    {
    }

void viewer::checkNodes()
    {
    // check we only have one viewer ( could not after a paste )
    nodeEntity *ent1( 0 );
    nodeEntity *ent2( 0 );
    // disable all other viewers
    AEntitySearch search( app()->search().findHasProperty( "nodeEntity/isNode" ) );
    foreach( AEntity *e, search )
        {
        nodeEntity *tempNode( e->castTo<nodeEntity*>() );
        if( tempNode->viewNode() == 1 )
            {
            ent1 = tempNode;
            }
        else if( tempNode->viewNode() == 2 )
            {
            ent2 = tempNode;
            }
        tempNode->setViewNode( 0 );
        }
    //primary renderer
    if( ent1 )
        {
        ent1->castTo<nodeEntity*>()->setViewNode( 1 );
        // set the current view node to ent.
        _renderer.setNode( ent1 );
        }
    //secondary renderer
    if( ent2 )
        {
        ent2->castTo<nodeEntity*>()->setViewNode( 2 );
        }
    }

void viewer::resetView()
    {
    //Reset the view transform
    _view = QTransform();
    _transformNeedsUpdate = TRUE;
    update();
    }

void viewer::oneToOne()
    {
    float wid = (float)width() / 1000.0;

    //Reset the view transform
    _view = QTransform();
    //Scale to fit width
    _view.scale( wid, wid );

    _transformNeedsUpdate = TRUE;
    update();
    }

void viewer::zoomToFit()
    {
    float wid( _renderer.rect().width() );

    wid = wid / 1000;

    //Reset the view transform
    _view = QTransform();
    //Scale to fit width
    _view.scale( wid, wid );

    _transformNeedsUpdate = TRUE;
    update();
    }

void viewer::entityAdded( AEntity *ent )
    {
    // connect to the node incase it wants to be viewed
    if( ent->propertyExists("nodeEntity/isNode") )
        {
        connect( ent, SIGNAL(_requestSetViewNode( nodeEntity *, int )), this, SLOT(setViewNode( nodeEntity *, int )) );
        }
    }

void viewer::entityRemoved( AEntity *ent )
    {
    // stop the renderer if were removing the viewed node from the array
    if( ent->propertyExists("nodeEntity/isNode") && ent->castTo<nodeEntity*>()->viewNode( ) == TRUE )
        {
        if( ent->castTo<nodeEntity*>()->viewNode( ) == 1 )
            {
            _renderer.setNode( 0 );
            }
        if( ent->castTo<nodeEntity*>()->viewNode( ) == 2 )
            {
            _rendererSecondary.setNode( 0 );
            }
        }
    }

void viewer::setViewNode( nodeEntity *node, int in )
    {
    // disable all other viewers
    AEntitySearch search( app()->search().findHasProperty( "nodeEntity/isNode" ) );
    foreach( AEntity *e, search )
        {
        if( e->castTo<nodeEntity*>()->viewNode() == in )
            {
            e->castTo<nodeEntity*>()->setViewNode( 0 );
            }
        }
    if( node )
        {
        node->setViewNode( in );
        }
    if( in == 1 )
        {
        // set the current view node to ent.
        _renderer.setNode( node );
        }
    else if( in == 2 )
        {
        // set the current view node to ent.
        _rendererSecondary.setNode( node );
        }
    initiateManipulators();
    }

void viewer::resizeEvent( QResizeEvent *event )
    {
    // resize the view to fit
    resizeView( event->size().width(), event->size().height() );
    }

void viewer::contextMenuEvent( QContextMenuEvent *event )
    {
    // stop menus appearing
    event->accept();
    }

void viewer::focusOutEvent( QFocusEvent * )
    {
    _activePickerMode = None;
    _activePicker = -1;
    setMouseTracking( false );
    }

void viewer::setHorizontallySplit( bool horiz )
    {
    _isHorizontallySplit = horiz;
    update();
    }

void viewer::mousePressEvent( QMouseEvent *event )
    {
    if( _activePickerMode == None )
        {
        bool found( false );
        QPointF pt( _mouseTransform.map( event->pos() ) );
        for( int x=0; x<_colourSamplerPositions.size(); x++ )
            {
            if( findDist( QPointF( event->pos() ), _painterTransform * _colourSamplerPositions[x] ) < 10 )
                {
                _activePickerMode = Deleting;
                _activePicker = x;
                found = TRUE;
                }
            }

        if( found == false )
            {
            if( event->pos().x() > (width() - 20) && !_isHorizontallySplit )
                {
                _splitPosChanging = true;
                _splitPos.setY( ( _mouseTransform * event->pos() ).y() );
                update();
                }
            else if( event->pos().y() > (height() - 20) && _isHorizontallySplit )
                {
                _splitPosChanging = true;
                _splitPos.setX( ( _mouseTransform * event->pos() ).x() );
                update();
                }
            else
                {
                // grab manipulator, or setup for transform
                _activeNode = 0;
                _activeManipulator = -1;
                _previousCoord = _mouseTransform.map( event->posF() );

                QPoint pt( _previousCoord.toPoint() );
                QMouseEvent ev( event->type(), pt, pt, event->button(), event->buttons(), event->modifiers() );
                foreach( AEntity *sel, app()->selection() )
                    {
                    nodeEntity *node( sel->castTo<nodeEntity*>() );
                    if( node )
                        {
                        if( node->canUse( &ev ) )
                            {
                            _activeNode = node;
                            _activeManipulator = -1;
                            }
                        }
                    }

                if( _activeNode == 0 )
                    {
                    foreach( AEntity *sel, app()->selection() )
                        {
                        nodeEntity *node( sel->castTo<nodeEntity*>() );
                        if( node )
                            {
                            int x=0;
                            for( int y=0; y<node->manipulators().size(); y++ )
                                {
                                nodeEntity::Manipulator manip( node->getManipulator( y ) );
                                if( manip.canUse( _previousCoord ) )
                                    {
                                    // found a manipulator
                                    _activeNode = node;
                                    _activeManipulator = x;
                                    node->setManipulator( y, manip );
                                    }
                                x++;
                                }
                            }
                        }
                    }

                _zoomTarget = _mouseTransform * event->posF();
                _oldPos = event->pos();
                event->accept();
                }
            }
        }
    }

void viewer::mouseMoveEvent( QMouseEvent *event )
    {
    QPointF nowCoord( _mouseTransform.map( event->posF() ) );
    if( _activePickerMode != None )
        {
        if( _activePickerMode == Moving && _activePicker >= 0 )
            {
            _colourSamplerPositions[_activePicker] = nowCoord.toPoint();
            update();
            }
        }
    else
        {
        if( _splitPosChanging && !_isHorizontallySplit )
            {
            _splitPos.setY( nowCoord.y() );
            update();
            }
        else if( _splitPosChanging && _isHorizontallySplit )
            {
            _splitPos.setX( nowCoord.x() );
            update();
            }
        else
            {

            if( (event->buttons()&Qt::LeftButton) != FALSE )
                {
                // do a colour sample
                _isSampling = true;
                emit colourSampleChanged( _renderer.sampleColourAt( (nowCoord-QPointF(0.5,0.5)).toPoint() ) );
                }

            if( _activeNode != 0 ) // we are manipulating
                {
                if( _activeManipulator != -1 )
                    {
                    _transforming = TRUE;

                    nodeEntity::Manipulator manip( _activeNode->getManipulator( _activeManipulator ) );
                    manip.use( nowCoord, _previousCoord );
                    _activeNode->setManipulator( _activeManipulator, manip );

                    _activeNode->manipulatorChanged( _activeManipulator );
                    _transforming = FALSE;

                    _previousCoord = nowCoord;

                    update();
                    }
                else
                    {
                    QPoint pt( nowCoord.toPoint() );
                    QMouseEvent ev( event->type(), pt, pt, event->button(), event->buttons(), event->modifiers() );
                    _activeNode->use( &ev );
                    }
                }
            else
                {
                if( ( (event->buttons()&Qt::MidButton) == Qt::MidButton || (event->buttons()&(Qt::LeftButton|Qt::RightButton)) == (Qt::LeftButton|Qt::RightButton) ) ) // translate view
                    {
                    translateView( ( _mouseTransform * _oldPos ) - ( _mouseTransform * event->pos() ) );
                    _oldPos = event->pos();
                    update();
                    }
                else if( (event->buttons()&Qt::RightButton) != FALSE && event->modifiers() == Qt::AltModifier ) // zoom view.
                    {
                    float xCmp((float)( event->pos().x() - _oldPos.x() )/(float)width()), yCmp((float)( event->pos().y() - _oldPos.y() )/(float)height());
                    float factor( xCmp + yCmp );
                    zoomView( factor, _zoomTarget );
                    _oldPos = event->pos();
                    update();
                    }
                }
            }
        }
    }

void viewer::mouseReleaseEvent( QMouseEvent *event )
    {
    QPoint nowCoord( _mouseTransform.map( event->pos() ) );
    if( _activePickerMode != None )
        {
        if( _activePickerMode == Picking )
            {
            _colourSamplers << nowCoord;
            _colourSamplerPositions << nowCoord;
            update();
            setMouseTracking( true );
            _activePickerMode = Moving;
            _activePicker = _colourSamplers.size() - 1;
            }
        else if( _activePickerMode == Moving )
            {
            update();
            setMouseTracking( false );
            _activePickerMode = None;
            _activePicker = -1;
            }
        else if( _activePickerMode == Deleting )
            {
            if( findDist( QPointF( event->pos() ), _painterTransform * _colourSamplerPositions[_activePicker] ) < 10 )
                {
                _colourSamplers.removeAt( _activePicker );
                _colourSamplerPositions.removeAt( _activePicker );
                }
            update();
            _activePickerMode = None;
            _activePicker = -1;
            }
        }
    else
        {
        if( _activeNode && _activeManipulator == -1 )
            {
            QMouseEvent ev( event->type(), nowCoord, nowCoord, event->button(), event->buttons(), event->modifiers() );
            _activeNode->use( &ev );
            }
        if( _isSampling )
            {
            _isSampling = false;
            QPointF nowCoord( _mouseTransform.map( event->posF() ) );
            emit colourSampleChanged( _renderer.sampleColourAt( (nowCoord-QPointF(0.5,0.5)).toPoint() ) );
            }
        _splitPosChanging = false;
        _activeNode = 0;
        _activeManipulator = -1;\
        initiateManipulators();
        }
    }

void viewer::wheelEvent( QWheelEvent *event )
    {
    if( event->buttons() == false )
        {
        zoomView( (float)event->delta() / 1200.0, _mouseTransform * event->pos() );
        }
    }

void viewer::initiateManipulators()
    {
    if( !_activeNode )
        {
        QPointF unScale( sqrt( ( _painterTransform.m11() * _painterTransform.m11() ) + ( _painterTransform.m21() * _painterTransform.m21() ) ),
                        sqrt( ( _painterTransform.m12() * _painterTransform.m12() ) + ( _painterTransform.m22() * _painterTransform.m22() ) ) );

        foreach( AEntity *sel, app()->selection() )
            {
            nodeEntity *node( sel->castTo<nodeEntity*>() );
            if( node )
                {
                // init manipulators from properties if we arent currently manipulating
                node->initiateManipulators();

                for( int x=0; x<node->manipulators().size(); x++ )
                    {
                    nodeEntity::Manipulator manip( node->getManipulator( x ) );
                    manip.setViewScale( unScale );
                    node->setManipulator( x, manip );
                    }
                }
            }
        }
    }

void viewer::paintEvent( QPaintEvent * )
    {
    GLE_QUIET; // discard any errors so far.
    makeCurrent() GLE;

    // clear view to black
    glClearColor( 0.0, 0.0, 0.0, 0.0 ) GLE;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) GLE;
    jLib::glewInitiate();

    // begin 2D painting too.
    QPainter pai( this );

    setViewTransform();

    QPointF splitPosScreen( _painterTransform * _splitPos );

    if( _isSplitEnabled )
        {
        if( _isHorizontallySplit )
            {
            QPixmap widg(":/calc/viewport/splitWidgetV.svg");
            pai.drawPixmap( splitPosScreen.x()-(widg.width()/2), height()-widg.height(), widg );
            }
        else
            {
            QPixmap widg(":/calc/viewport/splitWidget.svg");
            pai.drawPixmap( width()-widg.width(), splitPosScreen.y()-(widg.height()/2), widg );
            }
        }

    // set up transform
    pai.setRenderHint( QPainter::Antialiasing, true );
    pai.setTransform( _painterTransform );

    if( _cache.contains(_frame) )
        {
        pai.drawImage( _renderer.rect(), _cache[_frame] );

        // draw around active area
        pai.setPen( Qt::red );
        pai.drawRect( _renderer.rect() );
        }
    else
        {
        jLib::graphics::pixel::texture *tex( &_renderer.texture() ); // grab the texture
        jLib::graphics::pixel::texture *tex2( &_rendererSecondary.texture() ); // grab the texture

        pai.beginNativePainting();
        if( tex->isValid() )
            {
            glColor4f( 1.0, 1.0, 1.0, 1.0 );

            //Primary Renderer
                {
                float hFade( 0 );
                float vFade( 0 );
                QRectF rct( _renderer.rect() );
                if( _isSplitEnabled )
                    {
                    if( _isHorizontallySplit && _splitPos.x() > rct.left() )
                        {
                        float newLeft( qMax( _splitPos.x(), rct.left() ) );
                        hFade = 1.0-((rct.right()-newLeft)/rct.width());
                        rct.setLeft( newLeft );
                        }
                    else if( !_isHorizontallySplit && _splitPos.y() > rct.top() )
                        {
                        float newTop( qMax( _splitPos.y(), rct.top() ) );
                        vFade = 1.0-((rct.bottom()-newTop)/rct.height());
                        rct.setTop( newTop );
                        }
                    }

                if( !rct.isEmpty() )
                    {
                    glEnable( GL_TEXTURE_2D );
                    tex->bind();
                    glBegin( GL_QUADS );
                        glTexCoord2f( hFade, vFade );
                        glVertex3f( rct.left(), rct.top(), 0 );
                        glTexCoord2f( hFade, 1 );
                        glVertex3f( rct.left(), rct.bottom(), 0 );
                        glTexCoord2f( 1, 1 );
                        glVertex3f( rct.right(), rct.bottom(), 0 );
                        glTexCoord2f( 1, vFade );
                        glVertex3f( rct.right(), rct.top(), 0 );
                    glEnd();
                    tex->unbind();
                    }
                }

            //Secondary Renderer
            if( _isSplitEnabled )
                {
                float hFade( 1 );
                float vFade( 1 );
                QRectF rct( _rendererSecondary.rect() );

                if( _isHorizontallySplit && _splitPos.x() < ( rct.height() + rct.height() ) )
                    {
                    float newWidth( qMin( _splitPos.x() - rct.left(), rct.width() ) );
                    hFade = newWidth/rct.width();
                    rct.setWidth( newWidth );
                    }
                else if( !_isHorizontallySplit && _splitPos.y() < ( rct.top() + rct.height() ) )
                    {
                    float newHeight( qMin( _splitPos.y() - rct.top(), rct.height() ) );
                    vFade = newHeight/rct.height();
                    rct.setHeight( newHeight );
                    }

                if( !rct.isEmpty() )
                    {
                    glEnable( GL_TEXTURE_2D );
                    tex2->bind();
                    glBegin( GL_QUADS );
                        glTexCoord2f( 0, 0 );
                        glVertex3f( rct.left(), rct.top(), 0 );
                        glTexCoord2f( 0, vFade );
                        glVertex3f( rct.left(), rct.bottom(), 0 );
                        glTexCoord2f( hFade, vFade );
                        glVertex3f( rct.right(), rct.bottom(), 0 );
                        glTexCoord2f( hFade, 0 );
                        glVertex3f( rct.right(), rct.top(), 0 );
                    glEnd();
                    tex2->unbind();
                    }
                }
            }

        pai.endNativePainting();

        // draw around active area
        pai.setPen( Qt::green );
        pai.drawRect( _renderer.rect() );

        pai.drawLine( 0, 10, 0, -10 );
        pai.drawLine( 10, 0, -10, 0 );

        // set up manipulator pen
        QPen pen( Qt::yellow );
        pen.setWidthF( 1.5 );
        pai.setPen( pen );

        foreach( AEntity *sel, app()->selection() )
            {
            nodeEntity *node( sel->castTo<nodeEntity*>() );
            if( node )
                {
                node->paint( &pai );
                }
            }

        // Draw in Screen space
        pai.resetTransform();

        for( int x=0; x<_colourSamplers.size(); x++ )
            {
            QPoint smplPos( _painterTransform * _colourSamplers[x] );
            QPoint drawPos( _painterTransform * _colourSamplerPositions[x] );

            pai.setBrush( Qt::transparent );
            pai.setPen( Qt::white );
            pai.drawLine( smplPos, drawPos );

            XColour col( _renderer.sampleColourAt( _colourSamplers[x] ) );

            pai.setPen( Qt::transparent );
            int radius, startAngle;

            radius = 10 + col.r() * 20;
            startAngle = 0;
            pai.setBrush( Qt::red );
            pai.drawPie( QRect( drawPos-QPoint(radius,radius), QSize( radius*2, radius*2 ) ), startAngle * 16, 90 * 16 );

            radius = 10 + col.g() * 20;
            startAngle = 90;
            pai.setBrush( Qt::green );
            pai.drawPie( QRect( drawPos-QPoint(radius,radius), QSize( radius*2, radius*2 ) ), startAngle * 16, 90 * 16 );

            radius = 10 + col.b() * 20;
            startAngle = 180;
            pai.setBrush( Qt::blue );
            pai.drawPie( QRect( drawPos-QPoint(radius,radius), QSize( radius*2, radius*2 ) ), startAngle * 16, 90 * 16 );

            radius = 10 + col.a() * 20;
            startAngle = 270;
            pai.setBrush( Qt::white );
            pai.drawPie( QRect( drawPos-QPoint(radius,radius), QSize( radius*2, radius*2 ) ), startAngle * 16, 90 * 16 );

            pai.setBrush( Qt::black );
            pai.drawEllipse( drawPos, 10, 10 );
            pai.setPen( Qt::white );
            pai.setBrush( col.toLDRColour() );
            pai.drawEllipse( drawPos, 10, 10 );
            }
        }




    pai.end();
    }

void viewer::setSplitEnabled( bool en )
    {
    _isSplitEnabled = en;
    update();
    }

void viewer::translateView( QPointF trans )
    {
    QTransform tX;
    tX.translate( trans.x(), trans.y() );

    _view = _view * tX;

    _transformNeedsUpdate = TRUE;
    update();
    }

void viewer::zoomView( float zF, QPointF tx )
    {
    QTransform fw;
    fw.translate( tx.x(), tx.y() );
    QTransform bw;
    bw.translate( -tx.x(), -tx.y() );

    QTransform zoom;
    zoom.scale( (1-zF), (1-zF) );

    _view = _view * bw * zoom * fw;

    _transformNeedsUpdate = TRUE;
    update();
    }

void viewer::resizeView( float x, float y )
    {
    // resize the view
    _persp = QTransform();
    _persp.scale( 1.0, y/x );
    //_persp.setScale( jLib::math::triple( 1.0, (y/x), 1.0 ) );
    _transformNeedsUpdate = TRUE;
    update();
    }

QTransform viewer::aspectTransform()
    {
    return _globalAdjustment * _persp;
    }

QTransform viewer::fullTransform()
    {
    return aspectTransform() * _view;
    }

void viewer::updateViewTransform()
    {
    _transformNeedsUpdate = TRUE;
    }

void viewer::setViewTransform()
    {
    // if we need to recalculate the view transform
    if( _transformNeedsUpdate )
        {
        // calculate the painter transform for 2D elements
        QTransform viewport;
        viewport.scale( 1.0/width(), 1.0/height() );

        _mouseTransform = viewport * fullTransform();
        _painterTransform = _mouseTransform.inverted();

        initiateManipulators();

        _transformNeedsUpdate = FALSE;
        }
    }

void viewer::setDisplayMode( nodeRenderer::DisplayModeEnum in )
    {
    _renderer.setRenderMode( in );
    _rendererSecondary.setRenderMode( in );
    }

void viewer::refreshViewNodeFromArray()
    {
    // here we grab the node marked as viewable from the array ( after a file load for instance )
    AEntitySearch search( app()->search().findHasProperty( "nodeEntity/isNode" ) );
    foreach( AEntity *e, search )
        {
        if( e->property("isViewNode")->toInt() != 0 )
            {
            setViewNode( e->castTo<nodeEntity*>(), e->property("isViewNode")->toInt() );
            break;
            }
        }
    }

void viewer::setDisplayTime( int t )
    {
    _frame = t;
    if( !_cache.contains( t ) )
        {
        _renderer.setTime( t );
        _rendererSecondary.setTime( t );
        }
    else
        {
        update();
        }
    }

bool viewer::isColourSampling() const
    {
    return _isSampling;
    }

void viewer::setPreMultiply( bool premult )
    {
    _renderer.setPreMultiply( premult );
    _rendererSecondary.setPreMultiply( premult );
    }

bool viewer::shouldPreMultiply() const
    {
    return _renderer.shouldPreMultiply();
    }

void viewer::setGlobalMultiplier( double dbl )
    {
    _renderer.setGlobalMultiplier( dbl );
    _rendererSecondary.setGlobalMultiplier( dbl );
    }

void viewer::newColourPicker()
    {
    setFocus( Qt::MouseFocusReason );
    _activePickerMode = Picking;
    _activePicker = -1;
    }

void viewer::clearCache()
    {
    _cache.clear();
    setDisplayTime( _frame );
    }

void viewer::setCache( int frame, QImage image )
    {
    _cache.insert( frame, image );
    }

void viewer::clearViewer()
    {
    _colourSamplers.clear();
    _colourSamplerPositions.clear();

    updateGL();
    }
