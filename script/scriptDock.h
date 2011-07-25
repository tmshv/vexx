#ifndef SCRIPTDOCK_H
#define SCRIPTDOCK_H

#include "baseDock.h"
#include "appScript.h"

namespace Ui {
    class scriptDock;
}

class QToolBar;
class highlighter;

class scriptDock : public baseDock
    {
    Q_OBJECT
public:
    scriptDock( );
    ~scriptDock( );
    void added( );
    appScript *core();

public slots:
    void run();

private:
    Ui::scriptDock *ui;
    QToolBar *_toolbar;
    highlighter *_highlighter;
    appScript _appScript;
    };

#endif // SCRIPTDOCK_H
