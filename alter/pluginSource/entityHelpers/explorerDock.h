#ifndef EXPLORERDOCK_H
#define EXPLORERDOCK_H

#include "AAbstractDock"
#include "AGlobal"
#include "ALog"
#include "AEntity"
#include "AEntitySearch"
#include "QTreeWidgetItem"

namespace Ui
{
    class explorerDock;
}

class explorerDock : public AAbstractDock
    {
    Q_OBJECT
public:
    explorerDock();

    void addItem( QString, AEntityType, QString="" );
    void clearItems( );
    void select( QString );

    virtual void added();

protected slots:
    void itemSelected( );
    void displayModeChanged(QString);

private slots:
    void selectionChanged( );
    void arrayChange( AEntity * );

private:
    QTreeWidgetItem *findListItem( AEntityLocation );
    QTreeWidgetItem *findTreeItem( AEntityLocation );
    QTreeWidgetItem *findTreeSubItem( QStringList, int, QTreeWidgetItem * );
    Ui::explorerDock *widget;
    bool _tree;
    };

#endif
