#ifndef TIMEDOCK_H
#define TIMEDOCK_H

#include "timeGlobal.h"
#include "AAbstractDock"
#include "timelineWidget.h"
#include "AEntity"

namespace Ui {
    class timeDock;
}

class TIME_EXPORT timeDock : public AAbstractDock
    {
    Q_OBJECT
public:
    timeDock( );
    ~timeDock();

    virtual void added();

    void onClearState();
    void onOpenState();

    AEntity *entity();

signals:
    void timeChanged( APropertyDevice *, AProperty * );

public slots:
    void play();
    void pause();
    void stepFw();
    void stepBw();
    void skipFw();
    void skipBw();
    void setTime( JFLOAT );

private slots:
    void entityChanged( APropertyDevice *, AProperty * );
    void timeChanged( JFLOAT );
    void ensureTimeEntity();

private:
    AEntity *_entity;
    timelineWidget *tL;
    Ui::timeDock *m_ui;
    bool _setting;
    QTimer *_timer;
    float _fps;
    bool _loop;
    float _timeNow;
    };

#endif // TIMEDOCK_H
