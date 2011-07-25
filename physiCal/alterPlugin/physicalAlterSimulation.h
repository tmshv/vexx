#ifndef PHYSICALALTERSIMULATION_H
#define PHYSICALALTERSIMULATION_H

#include "physicalAlterGlobal.h"
#include "abstractSimulator.h"
#include "viewportTool.h"

#include "system.h"

// The simulation manager, interfaces with the simulation plugin
class PHYSICALALTER_EXPORT physicalAlterSimulation : public abstractSimulator,
                                                     private viewportTool
    {
    Q_OBJECT
public:
    physicalAlterSimulation();
    ~physicalAlterSimulation();

    void added();


    virtual void setupSimulate();
    virtual void clearSimulationCache( );
    virtual void setDisplayMode( displayMode );
    virtual void setDisplayTime( JFLOAT );
    virtual void setStepTime( JFLOAT );
    virtual void simulateStep( );
    virtual JFLOAT maximumAvailableTime( ) const;
    QString currentTimeLogText( ) const;

private slots:
    void showTypesChanged();
    void inititialiseGL();

    void arrayChanged( AEntity * );
    void entityChanged( APropertyDevice *, AProperty * );

private:
    void update();
    cal::system *_system;
    jLib::graphics::entity::entityG _group;
    bool drawPoints;
    jLib::graphics::primitives::pointCloud _point;
    bool drawLines;
    jLib::graphics::primitives::lineCloud _line;
    bool drawTris;
    jLib::graphics::primitives::triangleCloud _tri;
    jLib::graphics::shader::surfaceShader *_shader;
    cal::frame _currentFrame;
    };

#endif // PHYSICALALTERSIMULATION_H
