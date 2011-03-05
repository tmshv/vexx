#ifndef NODETOOLBAR_H
#define NODETOOLBAR_H

#include "AAbstractToolBar"

class QFileSystemWatcher;

class nodeToolBar : public AAbstractToolBar
    {
    Q_OBJECT
public:
    nodeToolBar();
    virtual void added();

private slots:
    void create(QAction*);
    void rebuild();
    };

#endif // NODETOOLBAR_H
