#ifndef PROEPRTIESDOCK_H
#define PROEPRTIESDOCK_H

#include "AAbstractDock"
#include "AGlobal"
#include "APropertyManager"

class QVBoxLayout;

class propertiesDock : public AAbstractDock
    {
    Q_OBJECT
public:
    propertiesDock( );
    void added();

    void setMode( APropertyManager::propertyMode );

private slots:
    void selectionChanged();

private:
    void clear();
    APropertyManager::propertyMode _mode;
    QVBoxLayout *_layout;
    QWidget *_currentManager;
    };

#endif
