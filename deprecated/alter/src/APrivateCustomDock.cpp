#include "APrivateCustomDock.h"
#include "QToolButton"
#include "QBoxLayout"
#include "QPainter"
#include "../SDK/AAbstractDock"
#include "APrivateWindow.h"
#include "QMenu"
#include "QPixmap"
#include "QContextMenuEvent"

/** \cond INTERNAL
  */

/** \class APrivateCustomTitleBar
  \brief A titlebar for a custom dock, draws a nice gradient and buttons that is consistant across all platforms.
  */

/** Creates a titlebar with parent \p parent.
  */
APrivateCustomTitleBar::APrivateCustomTitleBar( QWidget *parent ) : QWidget( parent ), vertical( FALSE ), _bg( 0 )
    {
    // set up internal layout
    _layout = new QBoxLayout( QBoxLayout::LeftToRight, this );
    _layout->setContentsMargins( 2, 2, 2, 2 );
    _layout->setSpacing( 3 );
    setLayout( _layout );

    // Align everything at the end, by adding a stretch first
    _layout->addStretch( 0 );
    // add a pop out toolbutton
    _popOut = new QToolButton( this );
    _popOut->setIcon( QIcon( ":/app/popOut.png" ) );
    _popOut->setAutoRaise( TRUE );
    connect( _popOut, SIGNAL(clicked()), this, SLOT(floatClick()) );
    _layout->addWidget( _popOut );
    // add a close toolbutton
    QToolButton *close = new QToolButton( this );
    close->setIcon( QIcon( ":/app/close.png" ) );
    close->setAutoRaise( TRUE );
    QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
    connect( close, SIGNAL(clicked()), dockWidget, SLOT(close()) );
    _layout->addWidget( close );
    }

/** Delete the titlebar
  */
APrivateCustomTitleBar::~APrivateCustomTitleBar()
    {
    if( _bg )
        {
        delete _bg;
        }
    }

/** Paint the titlebar.
  */
void APrivateCustomTitleBar::paintEvent( QPaintEvent * )
    {
    QPainter pai( this );
    pai.drawPixmap( 0, 0, *_bg );
    }

/** Causes the titlebar to re cache its titlebar pixmap.
  */
void APrivateCustomTitleBar::resizeEvent( QResizeEvent * )
    {
    // default direction
    vertical = FALSE;
    _layout->setDirection( QBoxLayout::LeftToRight );

    // if we are wrong, reset the direction
    QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
    if( ( dockWidget->features() & QDockWidget::DockWidgetVerticalTitleBar ) != FALSE )
        {
        _layout->setDirection( QBoxLayout::BottomToTop );
        vertical = TRUE;
        }

    // delte the background that is already there
    if( _bg )
        {
        delete _bg;
        }
    // reallocate the background
    _bg = new QPixmap( size() );

    // painter for background
    QPainter pai( _bg );
    QRect rct = rect();
    // setup automatic vertical drawing
    if( vertical )
        {
        QSize s = rct.size();
        s.transpose();
        rct.setSize(s);

        pai.translate(rct.left(), rct.top() + rct.width());
        pai.rotate(-90);
        pai.translate(-rct.left(), -rct.top());
        }

    // background gradient
    QLinearGradient grad( QPointF( 0, 0 ), QPointF( 0, rct.height() ) );
    grad.setColorAt( 0, QColor( 145, 145, 145 ) );
    grad.setColorAt( 0.1, QColor( 123, 123, 123 ) );
    grad.setColorAt( 0.4, QColor( 106, 106, 106 ) );
    grad.setColorAt( 0.41, QColor( 90, 90, 90 ) );
    grad.setColorAt( 1, QColor( 64, 64, 64 ) );
    pai.fillRect( rct, grad );

    // paint title text
    QFontMetrics fM( pai.font() );
    QString text( fM.elidedText(dockWidget->windowTitle(), Qt::ElideRight, rct.width() - 65 ) );
    pai.setPen( Qt::white );
    pai.drawText( rct.left() + 5, rct.top() + 3, rct.width() - 10, rct.height() - 6,
                    Qt::AlignVCenter, text );
    }

/** Returns the minimum size of the titlebar
  */
QSize APrivateCustomTitleBar::minimumSizeHint() const
    {
    return sizeHint();
    }

/** Returns the recommended size of the titlebar
  */
QSize APrivateCustomTitleBar::sizeHint() const
    {
    // different for vertical
    if( vertical )
        {
        return QSize( 25, 65 );
        }
    return QSize( 65, 25 );
    }

/** Toggle the floating status of the titlebar
  */
void APrivateCustomTitleBar::floatClick()
    {
    // flip value
    QDockWidget *dockWidget = qobject_cast<QDockWidget*>(parentWidget());
    dockWidget->setFloating(!dockWidget->isFloating());

    //setup icon for drawing
    if( dockWidget->isFloating() )
        {
        _popOut->setIcon( QIcon( ":/app/popIn.png" ) );
        }
    else
        {
        _popOut->setIcon( QIcon( ":/app/popOut.png" ) );
        }
    }

/** \class APrivateCustomDockWidget
    \brief A custom dock widget allows an abstract dock to be put into a QDockWidget.
  */

/** Create a custom dock widget for dock \p in, with paretn \p parent.
  */
APrivateCustomDockWidget::APrivateCustomDockWidget( AAbstractDock *in, QWidget *parent ) : QDockWidget( in->name(), parent ), _dock( in )
    {
    // setup name for saving state
    setObjectName( in->name() );
    setWidget( in->base() );
    // setup features
    QDockWidget::DockWidgetFeatures features( QDockWidget::DockWidgetClosable | QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable );
    features |= in->hasVerticalTitlebar() ? QDockWidget::DockWidgetVerticalTitleBar : QDockWidget::NoDockWidgetFeatures;
    setFeatures( features );
    // add custom titlebar
    setTitleBarWidget( new APrivateCustomTitleBar( this ) );
    // on qt windowing system ( is thie needed... )
#ifdef Q_WS_QWS
    // setup palette differently ( i think thi swas cut from the qt example... )
    QPalette pal = palette();
    pal.setBrush(backgroundRole(), QColor(0,0,0,0));
    swatch->setPalette(pal);
#endif
    }

/** Triggers a context menu for the dock widget
  */
void APrivateCustomDockWidget::contextMenuEvent( QContextMenuEvent *event )
    {
    // setup the main window dock, then trigger the window menu
    _dock->app()->_controller->setContextDock( _dock );
    _dock->app()->_controller->windowMenu()->exec( event->globalPos() );
    _dock->app()->_controller->setContextDock( 0 );
    }

/** \endcond
  */
