#include "viewerDock.h"
#include "QToolBar"
#include "QComboBox"
#include "QSpinBox"
#include "timelineWidget.h"
#include "colourSampleWidget.h"
#include "nodeEntity.h"
#include "colourPickerWidget.h"
#include "QAction"
#include "XFloatWidget"
#include "PreferencesDialog.h"
#include "QToolButton"
#include "QSettings"
#include "QTimer"
#include "QProgressBar"
#include "QStackedWidget"
#include "QImage"
#include "exportWidget.h"

viewerDock *globalViewer = 0;

QList <colourPickerWidget *> viewerDock::_colourPickers;

viewerDock::viewerDock() : _nodeTools( 0 )
    {
    _timer = new QTimer();
    connect( _timer, SIGNAL(timeout()), this, SLOT(stepFrame()) );

    QSettings settings;
    settings.setValue( "freeTextureUnits", QVariant() );
    }

viewerDock::~viewerDock()
    {
    delete _timer;
    }

void viewerDock::added()
    {
    globalViewer = this;
    _preferences = new PreferencesDialog( base() );

    // setup viewport layout
    _layout = new QVBoxLayout();
    _layout->setContentsMargins( 0, 0, 0, 0 );
    _layout->setSpacing( 0 );

    // set up ui
    _ui = new viewer( base(), app() );
    base()->setLayout( _layout );
    _layout->addWidget( _ui );

    _stack = new QStackedWidget( base() );
    _stack->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    _layout->addWidget( _stack );

    QWidget *tools( new QWidget );
    _toolLayout = new QHBoxLayout( tools );
    _toolLayout->setContentsMargins( 0, 0, 0, 0 );
    _stack->addWidget( tools );

    // add toolbar
    QToolBar *toolbar( new QToolBar( "Viewer Tools" ) );
    _toolLayout->addWidget( toolbar );

    XFloatWidget *mult( new XFloatWidget( 1.0 ) );
    mult->setMaximumWidth( 150 );
    toolbar->addWidget( mult );
    connect( mult, SIGNAL(valueChanged(double)), _ui, SLOT(setGlobalMultiplier(double)) );

    // add rgb chooser
    _box = new QComboBox;
    _box->addItem( QIcon( ":/calc/viewport/rgb.png" ), "RGB" );
    _box->addItem( QIcon( ":/calc/viewport/r.png" ), "Red" );
    _box->addItem( QIcon( ":/calc/viewport/g.png" ), "Green" );
    _box->addItem( QIcon( ":/calc/viewport/b.png" ), "Blue" );
    _box->addItem( QIcon( ":/calc/viewport/a.png" ), "Alpha" );// << "Red" << "Green" << "Blue" << "Alpha" );
    connect( _box, SIGNAL(activated(QString)), this, SLOT(changeDisplayMode(QString)) );
    toolbar->addWidget( _box );

    toolbar->addSeparator();

    // add view control buttons
    toolbar->addAction( QIcon(":/calc/viewport/zoomToFit.svg"), "Zoom to fit", _ui, SLOT(zoomToFit()) );
    toolbar->addAction( QIcon(":/calc/viewport/oneToOne.svg"), "Show 1:1", _ui, SLOT(oneToOne()) );

    _splitMode = 0;
    _split = toolbar->addAction( QIcon(":/calc/viewport/splitEnable.svg"), "Split Horizontal" );
    _split->setCheckable( true );
    connect( _split, SIGNAL(triggered()), this, SLOT(toggleSplit()) );

    QAction *alphaMult( toolbar->addAction( QIcon(":/calc/viewport/autoAlphaMult.svg"), "Pre Multiply Output" ) );
    alphaMult->setCheckable( true );
    connect( alphaMult, SIGNAL(triggered(bool)), _ui, SLOT(setPreMultiply(bool)) );
    _ui->setPreMultiply( alphaMult->isChecked() );

    QAction *colourPicker( toolbar->addAction( QIcon(":/calc/colourPicker.svg"), "Add Persistant Colour Picker" ) );
    connect( colourPicker, SIGNAL(triggered(bool)), _ui, SLOT(newColourPicker()) );

    QWidget *rendererProgress( new QWidget );
    _stack->addWidget( rendererProgress );
    QHBoxLayout *renderLayout = new QHBoxLayout( rendererProgress );
    renderLayout->setContentsMargins( 3, 3, 3, 3 );
    _stack->addWidget( rendererProgress );
    _progress = new QProgressBar( rendererProgress );
    _progress->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Maximum );
    renderLayout->addWidget( _progress );
    _closeRender = new QToolButton;
    _closeRender->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    _closeRender->setIcon( QIcon(":/calc/viewport/closeRender.svg") );
    connect( _closeRender, SIGNAL(clicked()), this, SLOT(closeRender()) );
    renderLayout->addWidget( _closeRender );

    // new timeline
    _timeline = new timelineWidget( base(), app() );

    QHBoxLayout *timeLayout( new QHBoxLayout() );
    _layout->addLayout( timeLayout );

    // minmum frame spinner
    QSpinBox *minTimeSpinner( new QSpinBox( base() ) );
    minTimeSpinner->setMaximumWidth( 50 );
    minTimeSpinner->setMinimumWidth( 50 );
    minTimeSpinner->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
    minTimeSpinner->setMaximumHeight( 25 );
    minTimeSpinner->setMinimum( -0xFFFF );
    minTimeSpinner->setMaximum( 0xFFFF );

    // max frame spinner
    QSpinBox *maxTimeSpinner( new QSpinBox( base() ) );
    maxTimeSpinner->setMaximumWidth( 50 );
    maxTimeSpinner->setMinimumWidth( 50 );
    maxTimeSpinner->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
    maxTimeSpinner->setMaximumHeight( 25 );
    maxTimeSpinner->setMinimum( -0xFFFF );
    maxTimeSpinner->setMaximum( 0xFFFF );

    // fps chooser
    QDoubleSpinBox *fpsSpinner( new QDoubleSpinBox( base() ) );
    fpsSpinner->setMaximumWidth( 60 );
    fpsSpinner->setMinimumWidth( 60 );
    fpsSpinner->setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Expanding );
    fpsSpinner->setMaximumHeight( 25 );
    fpsSpinner->setMinimum( 1.0 );
    fpsSpinner->setMaximum( HUGE_VAL );

    // commect spinners to timeline
    connect( minTimeSpinner, SIGNAL(valueChanged(int)), _timeline, SLOT(setMinimumFrame(int)) );
    connect( maxTimeSpinner, SIGNAL(valueChanged(int)), _timeline, SLOT(setMaximumFrame(int)) );
    connect( fpsSpinner, SIGNAL(valueChanged(double)), _timeline, SLOT(setFps(double)) );

    connect( _timeline, SIGNAL(currentTimeChanged(int)), _ui, SLOT(setDisplayTime(int)) );

    minTimeSpinner->setValue( _timeline->minimumFrame() );
    maxTimeSpinner->setValue( _timeline->maximumFrame() );
    fpsSpinner->setValue( _timeline->fps() );

    QToolButton *begin( new QToolButton() );
    begin->setIcon( QIcon( ":/calc/viewport/begin.svg"));
    begin->setMaximumSize( 25, 25 );
    connect( begin, SIGNAL(clicked()), this, SLOT(skipBegin()) );
    _playButton = new QToolButton();
    _playButton->setIcon( QIcon( ":/calc/viewport/play.svg"));
    _playButton->setMaximumSize( 25, 25 );
    _playButton->setCheckable( true );
    connect( _playButton, SIGNAL(clicked(bool)), this, SLOT(playPause(bool)) );
    QToolButton *end( new QToolButton() );
    end->setIcon( QIcon( ":/calc/viewport/end.svg"));
    end->setMaximumSize( 25, 25 );
    connect( end, SIGNAL(clicked()), this, SLOT(skipEnd()) );


    // add spinners and timeline to this
    timeLayout->addWidget( begin );
    timeLayout->addWidget( _playButton );
    timeLayout->addWidget( end );
    timeLayout->addWidget( minTimeSpinner );
    timeLayout->addWidget( _timeline );
    timeLayout->addWidget( maxTimeSpinner );
    timeLayout->addWidget( fpsSpinner );

    // add colour sample widget
    _colour = new colourSampleWidget( base() );
    connect( _ui, SIGNAL(colourSampleChanged(XColour)), _colour, SLOT(setColour(XColour)) );
    connect( _ui, SIGNAL(colourSampleChanged(XColour)), this, SLOT(colourPicked(XColour)) );
    timeLayout->addWidget( _colour );

    connect( app(), SIGNAL(selectionChanged()), this, SLOT(refreshNodeToolbar()) );
    }

void viewerDock::registerColourPicker( colourPickerWidget *in )
    {
    _colourPickers << in;
    }


void viewerDock::removeColourPicker( colourPickerWidget *in )
    {
    _colourPickers.removeAll( in );
    }

void viewerDock::colourPicked( XColour in )
    {
    bool isSampling( _ui->isColourSampling() );
    foreach( colourPickerWidget *picker, _colourPickers )
        {
        picker->colourPicked( in, !isSampling );
        }
    }

void viewerDock::highlightMode( int in )
    {
    // set combobox settings from the viewer
    if( in == nodeRenderer::DisplayRGBA )
        {
        _box->setCurrentIndex( 0 );
        }
    else if( in == nodeRenderer::DisplayRed )
        {
        _box->setCurrentIndex( 1 );
        }
    else if( in == nodeRenderer::DisplayGreen )
        {
        _box->setCurrentIndex( 2 );
        }
    else if( in == nodeRenderer::DisplayBlue )
        {
        _box->setCurrentIndex( 3 );
        }
    else if( in == nodeRenderer::DisplayAlpha )
        {
        _box->setCurrentIndex( 4 );
        }
    }

void viewerDock::changeDisplayMode( QString in )
    {
    // set viewer display mode from combo box
    if( in == "RGB" )
        {
        _ui->setDisplayMode( nodeRenderer::DisplayRGBA );
        }
    else if( in == "Red" )
        {
        _ui->setDisplayMode( nodeRenderer::DisplayRed );
        }
    else if( in == "Green" )
        {
        _ui->setDisplayMode( nodeRenderer::DisplayGreen );
        }
    else if( in == "Blue" )
        {
        _ui->setDisplayMode( nodeRenderer::DisplayBlue );
        }
    else if( in == "Alpha" )
        {
        _ui->setDisplayMode( nodeRenderer::DisplayAlpha );
        }
    }

void viewerDock::refreshNodeToolbar()
    {
    // remove and added view toolbars
    if( _nodeTools )
        {
        _toolLayout->removeWidget( _nodeTools );
        _nodeTools->setParent( 0 );
        _nodeTools->hide();
        _nodeTools = 0;
        }
    if( app()->selection().size() )
        {
        nodeEntity *node( app()->selection().front().castTo<nodeEntity *>() );
        if( node && node->toolbar() )
            {
            // add new toolbars
            _nodeTools = node->toolbar();

            _toolLayout->addWidget( _nodeTools );
            }
        }
    }

QAction *viewerDock::showPreferences()
    {
    // show preferences
    QAction *act( new QAction( "Preferences", base() ) );
    act->connect( act, SIGNAL(triggered()), _preferences, SLOT(show()) );
    return act;
    }

void viewerDock::skipBegin()
    {
    // shop the time to the start frame
    _timeline->setCurrentFrame( _timeline->minimumFrame() );
    _ui->setDisplayTime( _timeline->currentFrame() );
    }

void viewerDock::playPause( bool toggle )
    {
    // toggle play / pause
    _playButton->setIcon( QIcon( toggle ? ":/calc/viewport/pause.svg" : ":/calc/viewport/play.svg" ) );

    if( toggle )
        {
        _timer->start( 1000 / _timeline->fps() );
        }
    else
        {
        _timer->stop();
        }
    }

void viewerDock::skipEnd()
    {
    // set the time to the end
    _timeline->setCurrentFrame( _timeline->maximumFrame() );
    _ui->setDisplayTime( _timeline->currentFrame() );
    }

void viewerDock::stepFrame()
    {
    // step forward
    if( _timeline->currentFrame() + 1 > _timeline->maximumFrame() )
        {
        _timeline->setCurrentFrame( 0 );
        }
    else
        {
        _timeline->setCurrentFrame( _timeline->currentFrame() + 1 );
        }
    _ui->setDisplayTime( _timeline->currentFrame() );
    }

void viewerDock::addFrame( int now, int min, int max, QImage im )
    {
    // add a frame to the cache at now and set the render progress to reflect min/max
    globalViewer->_stack->setCurrentIndex( 1 );
    globalViewer->_progress->setValue( 100.0 * ( ((float)now-(float)min)/((float)max-(float)min) ) );
    globalViewer->_ui->setCache( now, im );
    globalViewer->_colour->setVisible( false );
    }

void viewerDock::closeRender()
    {
    // clear cache and update view
    _stack->setCurrentIndex( 0 );
    _ui->clearCache();
    _colour->setVisible( true );
    }

void viewerDock::installExport( exportWidget *widg )
    {
    connect( globalViewer->_closeRender, SIGNAL(clicked()), widg, SLOT(cancelRender()), Qt::UniqueConnection );
    }

void viewerDock::toggleSplit()
    {
    // toggle the split mode between none - horizontal - vertical...
    (++_splitMode) %= 3; // cycle 0, 1, 2, 0, 1, 2...
    _split->setChecked( _splitMode > 0 );

    if( _splitMode > 0 )
        {
        _ui->setSplitEnabled( true );
        if( _splitMode == 1 )
            {
            _ui->setHorizontallySplit( false );
            _split->setIcon( QIcon(":/calc/viewport/splitVertical.svg") );
            _split->setText( "Split Vertical" );
            }
        else
            {
            _ui->setHorizontallySplit( true );
            _split->setIcon( QIcon(":/calc/viewport/disableSplit.svg") );
            _split->setText( "Disable Split" );
            }
        }
    else
        {
        _ui->setSplitEnabled( false );
        _split->setIcon( QIcon(":/calc/viewport/splitEnable.svg") );
        _split->setText( "Split Horizontal" );
        }
    }
