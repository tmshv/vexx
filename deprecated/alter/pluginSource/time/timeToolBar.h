#ifndef TIMETOOLBAR_H
#define TIMETOOLBAR_H

#include "AAbstractToolBar"

class timeDock;
class QAction;

class timeToolBar : public AAbstractToolBar
    {
    Q_OBJECT
public:
    timeToolBar( timeDock * );

private slots:
    void playPause( );
    void skipBw( );
    void skipFw( );

private:
    QAction *_playPause;
    timeDock *_dock;
    };

#endif // TIMETOOLBAR_H
