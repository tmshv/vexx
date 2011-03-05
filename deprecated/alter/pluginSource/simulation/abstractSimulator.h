#ifndef ABSTRACTSIMULATOR_H
#define ABSTRACTSIMULATOR_H

#include "QObject"
#include "simulationGlobal.h"
#include "APlugin"

class SIMULATION_EXPORT abstractSimulator : public QObject
    {
    Q_OBJECT
    jProperty( APlugin *, app )
public:
    enum displayMode { Setup, Simulation };

    virtual void added();

    virtual void setupSimulate() = 0;
    virtual void clearSimulationCache( ) = 0;
    virtual void setDisplayMode( displayMode ) = 0;
    virtual void setDisplayTime( JFLOAT ) = 0;
    virtual void setStepTime( JFLOAT ) = 0;
    virtual void simulateStep( ) = 0;
    virtual JFLOAT maximumAvailableTime( ) const = 0;
    virtual QString currentTimeLogText( ) const;

protected:
    void log( QString );
    void setupChanged();

signals:
    void logSignal( QString );
    void setupChangedSignal();
    };

#endif // ABSTRACTSIMULATOR_H
