#ifndef SIMULATIONDOCK_H
#define SIMULATIONDOCK_H

#include "AGlobal"
#include "simulationGlobal.h"
#include "AAbstractToolBar"
#include "timeDock.h"

class QProgressBar;
class QCheckBox;
class frameLogDock;
class simulationManager;
class simThread;

class simulationToolBar : public AAbstractToolBar
    {
    Q_OBJECT
    jROProperty( simulationManager *, manager )
    jProperty( frameLogDock *, logDock )

public:
    simulationToolBar();
    ~simulationToolBar();

    void added();

public slots:
    void reset();
    void stop();

private slots:
    void simulateClicked();
    void updateTime( );
    void updateSimulationView( bool );
    void simulationComplete( );
    void log( QString );
    void timeChanged(APropertyDevice*,AProperty*);

signals:
    void simulationLog( QString );
    void stepped( bool );
    void finished( );

private:
    QAction *_simulate;
    void stopSimulating();
    void setDisplayTime( JFLOAT );
    JFLOAT maximumAvailableTime();
    void simulate();
    JFLOAT _simulationTarget;
    friend class simThread;
    simThread *_currentSimThread;
    QAction *_dragTime;
    QAtomicInt _stop;
    };

#endif // SIMULATIONDOCK_H
