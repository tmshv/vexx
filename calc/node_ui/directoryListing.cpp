#include "directoryListing.h"
#include "QPainter"
#include "gfxscene.h"
#include "gfxview.h"
#include "QGraphicsProxyWidget"
#include "QHBoxLayout"
#include "QPushButton"
#include "QToolButton"
#include "nodeDB.h"
#include "QLabel"

directoryListing::directoryListing( APlugin *plug, gfxview *view ) : _height( 40 ), _view( view ), _app( plug )
    {
    _proxy = new QGraphicsProxyWidget( this );

    connect( _view, SIGNAL(viewResized()), this, SLOT(resize()) );
    connect( _view, SIGNAL(viewScaled()), this, SLOT(resize()) );
    connect( _view, SIGNAL(viewMoved()), this, SLOT(resize()) );

    connect( _app, SIGNAL(workingPathChanged()), this, SLOT(refreshPath()) );
    connect( _app, SIGNAL(cleanState()), this, SLOT(refreshPath()) );
    connect( _app, SIGNAL(openState()), this, SLOT(refreshPath()) );

    setZValue( 40000 );

    refreshPath();
    }

void directoryListing::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
    {
    QLinearGradient grad( QPoint( 0, 0 ), QPoint( 0, _height ) );
    grad.setColorAt( 0.0, QColor( 85, 85, 85 ) );
    grad.setColorAt( 0.8, QColor( 55, 55, 55 ) );
    grad.setColorAt( 1.0, QColor( 25, 25, 25 ) );

    painter->fillRect( QRectF( 0, 0, _view->width( ), _height ), grad );
    }

QRectF directoryListing::boundingRect() const
    {
    return QRectF( 0, 0, scene()->views().front()->width( ), _height );
    }

void directoryListing::resize()
    {
    // we want NO transform
    setTransform( _view->transform().inverted() );

    // size the proxy widget to us too
    _proxy->setPos( 0, 0 );
    _proxy->widget()->resize( QSize( _view->width(), _height ) );

    // tell the scene were changing size
    prepareGeometryChange();
    }

void directoryListing::refreshPath()
    {
    QWidget *oldWidget( _proxy->widget() );
    QWidget *widg = new QWidget();
    _proxy->setWidget( widg );
    delete oldWidget;

    AEntityLocation loc( _app->workingPath() );

    if( loc.isRoot() )
        {
        setVisible( false );
        }
    else
        {
        setVisible( true );
        QHBoxLayout *layout( new QHBoxLayout( widg ) );
        layout->setContentsMargins( 4, 4, 4, 7 );

        QPalette pal( widg->palette() );
        pal.setColor( QPalette::Background, Qt::transparent );
        widg->setPalette( pal );

        QStringList levels( loc.levels() );

        QToolButton *rootBut( new QToolButton( widg ) );
        rootBut->setText( "/" );
        rootBut->setIconSize( QSize( 16, 16 ) );
        rootBut->setIcon( QIcon(":/calc/folder.png") );
        connect( rootBut, SIGNAL(clicked()), this, SLOT(changePath()) );
        layout->addWidget( rootBut );


        QString path;
        foreach( QString lev, levels )
            {
            path += "/" + lev;

            QLabel *label( new QLabel( widg ) );
            layout->addWidget( label );

            label->setPixmap( QPixmap( ":/calc/next.png" ) );

            QToolButton *but( new QToolButton( widg ) );
            but->setToolButtonStyle( Qt::ToolButtonTextBesideIcon );

            nodeEntity *node( _app->search().findEntity( path )[0].castTo<nodeEntity *>() );
            if( node )
                {
                but->setIconSize( QSize( 16, 16 ) );
                QIcon ico( node->icon() );
                if( ico.isNull() )
                    {
                    but->setIcon( QIcon(":/calc/folder.png") );
                    }
                else
                    {
                    but->setIcon( ico );
                    }
                }

            but->setText( lev );
            connect( but, SIGNAL(clicked()), this, SLOT(changePath()) );

            layout->addWidget( but );
            }

        layout->addStretch();
        }
    resize();
    }

void directoryListing::changePath()
    {
    QString path;
    foreach( QObject *obj, _proxy->widget()->children() )
        {
        QAbstractButton *but( qobject_cast<QAbstractButton *>(obj) );

        if( but )
            {
            path += "/" + but->text();
            }
        if( obj == sender() )
            {
            break;
            }
        }
    Q_EMIT changeDirectory( path );
    }

/** Return the type of this object, used for safe casting with qgraphicsitem_cast<XXX>()
  */
int directoryListing::type() const
    {
    return Type;
    }
