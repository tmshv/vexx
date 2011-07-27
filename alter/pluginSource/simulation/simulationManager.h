#ifndef SIMULATIONMANAGER_H
#define SIMULATIONMANAGER_H

#include "simulationGlobal.h"
#include "AGlobal"
#include "QObject"
#include "abstractSimulator.h"

class SIMULATION_EXPORT simulationManager : public QObject
    {
    Q_OBJECT
    jProperty( abstractSimulator::displayMode, mode )

public:
    simulationManager( );
    void setApp( APlugin * );

    void addSimulator( abstractSimulator * );
    const QList <abstractSimulator *> &simulators();

signals:
    void reset();
    void log( QString );

public slots:
    void removeSimulator( abstractSimulator * );

private slots:
    void removeSimulator( QObject * );

private:
    QList <abstractSimulator *> _simulators;
    APlugin *_app;
    };

#endif // SIMULATIONMANAGER_H
