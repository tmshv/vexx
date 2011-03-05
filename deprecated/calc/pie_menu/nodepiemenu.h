#ifndef NODEPIEMENU_H
#define NODEPIEMENU_H

#include "qtpiemenu.h"

class nodePieMenu : public QtPieMenu
{
public:
    nodePieMenu(const QString &text, QWidget *parent);
protected:
    void paintEvent(QPaintEvent *e);
};

#endif // NODEPIEMENU_H
