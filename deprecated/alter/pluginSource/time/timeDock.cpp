#include "timeDock.h"
#include "ui_timeDock.h"
#include "../SDK/ALog"
#include "AEntitySearch"
#include "timeEntity.h"
#include "QTimer"

timeDock::timeDock( ) : m_ui( new Ui::timeDock )
    {
    m_ui->setupUi( base() );
    tL = new timelineWidget( base() );
    m_ui->layout->addWidget( tL );

    setTitleBarPosition( AAbstractDock::Vertical );

    tL->timeInView( 10.0 );
    tL->beginTime( -100.0 );
    tL->endTime( 100.0 );
    tL->setTimeSeconds( 10.0 );
    tL->viewCentre( 0.0 );
    tL->clipBegin( 0.0 );
    tL->clipEnd( 10.0 );
    tL->connect( tL, SIGNAL(timeChanged(JFLOAT)), this, SLOT(timeChanged(JFLOAT)) );
    _timer = new QTimer( this );
    connect( _timer, SIGNAL(timeout()), this, SLOT(stepFw()));
    _fps = 25;
    _timeNow = 0;
    _setting = FALSE;
    _entity = 0;
    }

timeDock::~timeDock()
    {
    delete m_ui;
    }

void timeDock::setTime( JFLOAT in )
    {
    if( _entity )
        {
        _entity->property( "time/currentTime" ) = in;
        }
    timeChanged( in );
    }

void timeDock::timeChanged( JFLOAT time )
    {
    if( !_setting )
        {
        _setting = TRUE;
        if( _entity )
            {
            _entity->property( "time/currentTime" ) = time;
            }
        else
            {
            ALog << "Time changed, but i cant find an entity" << endl;
            }
        _setting = FALSE;
        }
    }

void timeDock::added()
    {
    connect( app(), SIGNAL(openState()), this, SLOT(ensureTimeEntity()) );
    connect( app(), SIGNAL(cleanState()), this, SLOT(ensureTimeEntity()) );
    ensureTimeEntity();
    }

void timeDock::entityChanged( APropertyDevice *time, AProperty * )
    {
    if( !_setting )
        {
        _setting = TRUE;
        tL->setTimeSeconds( time->property( "time/currentTime" )->toDouble() );
        tL->clipBegin( time->property( "time/clipBegin" )->toDouble() );
        tL->clipEnd( time->property( "time/clipEnd" )->toDouble() );
        tL->beginTime( time->property( "time/timeBegin" )->toDouble() );
        tL->endTime( time->property( "time/timeEnd" )->toDouble() );
        tL->fps( _fps = time->property( "time/fps" )->toDouble() );
        _loop = time->property( "time/loop" )->toBool();
        _timeNow = time->property( "time/currentTime" )->toDouble();
        _setting = FALSE;
        }
    }

void timeDock::play()
    {
    _timer->setInterval( 1000 * ( 1 / _fps ) );
    _timer->start();
    }

void timeDock::pause()
    {
    _timer->stop();
    }

void timeDock::skipFw()
    {
    pause();
    if( _entity )
        {
        setTime( _entity->property( "time/clipEnd" )->toDouble() );
        }
    }

void timeDock::skipBw()
    {
    pause();
    if( _entity )
        {
        setTime( _entity->property( "time/clipBegin" )->toDouble() );
        }
    }

void timeDock::stepFw()
    {
    if( tL->currentTime() < tL->clipEnd() )
        {
        if( _entity )
            {
            setTime( _entity->property( "time/currentTime" )->toDouble() + ( 1 / _fps ) );
            }
        }
    else if( _loop )
        {
        setTime( 0 );
        }
    else
        {
        setTime( tL->clipEnd() );
        pause();
        }
    }

void timeDock::stepBw()
    {
    if( tL->currentTime() > tL->clipBegin() )
        {
        if( _entity )
            {
            setTime( _entity->property( "time/currentTime" )->toDouble() - ( 1 / _fps ) );
            }
        }
    else if( _loop )
        {
        setTime( tL->clipEnd() );
        }
    else
        {
        setTime( tL->clipBegin() );
        pause();
        }
    }

void timeDock::ensureTimeEntity()
    {
    AEntitySearch timeEnts = app()->search().findType( timeEntity::identifier() );
    if( timeEnts.size() == 0 )
        {
        _entity = new timeEntity( );
        app()->addEntity( _entity );
        entityChanged( _entity, 0 );
        }
    else
        {
        FOREACH( timeEnts, t )
            {
            _entity = (*t);
            }
        }
    disconnect( _entity, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)) );
    disconnect( _entity, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SIGNAL(timeChanged(APropertyDevice*,AProperty*)) );
    connect( _entity, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)) );
    connect( _entity, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SIGNAL(timeChanged(APropertyDevice*,AProperty*)) );
    }

AEntity *timeDock::entity()
    {
    return _entity;
    }

void timeDock::onClearState()
    {
    ensureTimeEntity();
    }

void timeDock::onOpenState()
    {
    ensureTimeEntity();
    }
