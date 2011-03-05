#ifndef NODEEXPLORERDOCK_H
#define NODEEXPLORERDOCK_H

#include "baseDock.h"

class nodeExplorerWidget;
class QVBoxLayout;

class nodeExplorerDock : public baseDock
    {
public:
    nodeExplorerDock();

private:
    nodeExplorerWidget *_widget;
    QVBoxLayout *_layout;
    };

#endif // NODEEXPLORERDOCK_H
