#include "APrivateSplashScreen.h"
#include "QPainter"
#include "QIcon"
#include "QTimeLine"
#include "QTimer"
#include "QApplication"
#include "QDesktopWidget"

#include "QDebug"

/** \cond INTERNAL
  \class APrivateSplashScreen
  \brief APrivateSplashScreen provides an alpha-supporting splash screen for use while an application loads
  */

/** Construct the splash with a window icon of \p icon, and a splash image of \p in
  */
APrivateSplashScreen::APrivateSplashScreen( QIcon icon, QImage in ) : map( QPixmap::fromImage( in ) ), fading( FALSE ), instant( FALSE )
    {
    setWindowFlags( windowFlags() | Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint );

    setWindowIcon( icon );
    resize( map.size() );

    QDesktopWidget *screenWidg( QApplication::desktop() );
    QRect priScreen( screenWidg->screenGeometry( screenWidg->primaryScreen() ) );

    move( priScreen.center() - QPoint( ( map.size() * 0.5 ).width(), ( map.size() * 0.5 ).height() ) );

    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setAttribute( Qt::WA_TranslucentBackground, TRUE );
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setWindowOpacity( 0.0 );
    }

/** \overload
  Fades the splash in.
  */
void APrivateSplashScreen::showEvent( QShowEvent * )
    {
    fadeIn();
    }

/** \overload
  Paints the splash image.
  */
void APrivateSplashScreen::paintEvent( QPaintEvent * )
    {
    QPainter pai( this );
    pai.drawPixmap( QPoint( 0, 0 ), map );
    }

/** \overload
  Fades the splash out when the window is loaded.
  */
void APrivateSplashScreen::finish( QWidget *mainWin )
    {
    if (mainWin)
        {
#if defined(Q_WS_X11)
        extern void qt_x11_wait_for_window_manager(QWidget *mainWin);
        qt_x11_wait_for_window_manager(mainWin);
#endif
        }
    fadeOut();
    }

/** Fade the splash screen in
  */
void APrivateSplashScreen::fadeIn()
    {
    direction = In;

    //Construct a 1-second timeline with a frame range of 0 - 100
    QTimeLine *timeLine = new QTimeLine(1000, this);
    timeLine->setFrameRange(0, 100);
    connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(fadeTo(int)));
    connect(timeLine, SIGNAL(finished()), this, SLOT(in()) );
    timeLine->start();
    fading = TRUE;
    }

/** Fade the splash out
  */
void APrivateSplashScreen::fadeOut()
    {
    if( !fading )
        {
        direction = Out;

        //Construct a 1-second timeline with a frame range of 0 - 100
        QTimeLine *timeLine = new QTimeLine(500, this);
        timeLine->setFrameRange(0, 100);
        connect(timeLine, SIGNAL(frameChanged(int)), this, SLOT(fadeTo(int)));
        connect(timeLine, SIGNAL(finished()), this, SLOT(out()) );
        timeLine->start();
        fading = TRUE;
        }
    else
        {
        instant = TRUE;
        }
    }

/** Slot triggered when the splash is completely in
  */
void APrivateSplashScreen::in()
    {
    fading = FALSE;
    if( instant )
        {
        fadeOut();
        }
    }

/** Slot triggered when the splash is completely out
  */
void APrivateSplashScreen::out()
    {
    fading = FALSE;
    close();
    emit finished();
    }

/** Fade the splash to a specific opacity
  */
void APrivateSplashScreen::fadeTo( int level )
    {
    if( direction == In )
        {
        setWindowOpacity( level / 100.0 );
        }
    else if( direction == Out )
        {
        setWindowOpacity( 1 - ( level / 100.0 ) );
        }
    }

/** \endcond INTERNAL */
