#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include "AAbstractDock"
#include "XProperty"

namespace Ui {
    class debugDock;
}

class entityModel;
class APlugin;

class debugDock : public AAbstractDock
    {
    Q_OBJECT
public:
    debugDock();
    ~debugDock();

    virtual void added();

private:
    Ui::debugDock *ui;
    entityModel *_model;
    };

#endif // DEBUGDIALOG_H
