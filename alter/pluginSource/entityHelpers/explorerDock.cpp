#include "explorerDock.h"
#include "ui_explorerDock.h"

#define NAME_COL 0
#define DIR_COL 2

explorerDock::explorerDock() : _tree( TRUE )
    {
    widget = new Ui::explorerDock( );
    widget->setupUi( base() );

    connect( widget->explorer, SIGNAL(itemClicked(QTreeWidgetItem *,int)), this, SLOT(itemSelected()) );
    connect( widget->comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT( displayModeChanged(QString) ) );
    }

void explorerDock::added()
    {
    connect( app(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()) );

    connect( app(), SIGNAL(entityAdded(AEntity *)), this, SLOT(arrayChange(AEntity*)) );
    connect( app(), SIGNAL(entityRemoved(AEntity *)), this, SLOT(arrayChange(AEntity*)) );
    }

void explorerDock::displayModeChanged( QString in )
    {
    if( in == "Tree" )
        {
        _tree = TRUE;
        }
    else
        {
        _tree = FALSE;
        }
    arrayChange( 0 );
    }

void explorerDock::selectionChanged( )
    {
    //Selecting items in a tree view is hard...
    }

void explorerDock::arrayChange( AEntity * )
    {
    clearItems();
    AEntitySearch all = app()->search();
    int done = 0, pos = 0;
    do
        {
        AEntitySearch search = all.findLevel( pos++ );
        for( int x=0; x<search.size(); x++ )
            {
            if( !search[x].hidden() )
                {
                addItem( search[x].name(), search[x].type(), search[x].location().directory() );
                }
            done++;
            }
        } while( done < all.size() );
    }

void explorerDock::itemSelected( )
    {
    static bool isSelecting = 0;
    if( !isSelecting )
        {
        app()->selectClear();

        QList<QTreeWidgetItem *> items = widget->explorer->selectedItems();

        for( int i=0; i<items.size(); i++ )
            {
            AEntitySearch search( app()->search().findEntity( AEntityLocation( items[i]->text( DIR_COL ) + "/" + items[i]->text( NAME_COL ) ) ) );
            if( search.size() )
                {
                app()->selectAdd( &(search[0]) );
                }
            }
        }
    }

void explorerDock::addItem( QString n, AEntityType t, QString p )
    {
    QTreeWidgetItem *created = 0;
    if( p == "" || !_tree )
        {
        created = new QTreeWidgetItem( widget->explorer, QStringList() << n << t.name() << "/" , 0 );
        }
    else
        {
        QTreeWidgetItem *item = findTreeItem( p );
        if( item )
            {
            created = new QTreeWidgetItem( item, QStringList() << n << t.name() << p, 0 );
            }
        else
            {
            ALog<<"Parent tree '"<<p<<"' not found"<<endl;
            }
        }

    if( created )
        {
        created->setExpanded( TRUE );
        }

    widget->explorer->sortItems( 0, Qt::AscendingOrder );
    }

void explorerDock::clearItems( )
    {
    widget->explorer->clear();
    }

void explorerDock::select( QString in )
    {
    QTreeWidgetItem *item;
    if( _tree )
        {
        item = findTreeItem( in );
        }
    else
        {
        item = findListItem( in );
        }
    if( item )
        {
        widget->explorer->setCurrentItem( item );
        }
    }

QTreeWidgetItem *explorerDock::findListItem( AEntityLocation in )
    {
    for( int x=0; x<widget->explorer->topLevelItemCount(); x++ )
        {
        QTreeWidgetItem *currentElement = widget->explorer->topLevelItem( x );
        if( ( currentElement->text( NAME_COL ) + "/" + currentElement->text( DIR_COL ) ) == in )
            {
            return currentElement;
            }
        }
    return 0;
    }

QTreeWidgetItem *explorerDock::findTreeItem( AEntityLocation n )
    {
    QString fullPath = n.path();
    QStringList path = fullPath.split( "/", QString::SkipEmptyParts );

    QString rootString = fullPath;
    if( path.size() != 0 )
        {
        rootString = path[0];
        }

    QTreeWidgetItem *rootElement = 0;
    for( int x=0; x<widget->explorer->topLevelItemCount(); x++ )
        {
        QTreeWidgetItem *currentElement = widget->explorer->topLevelItem( x );
        if( currentElement->text( NAME_COL ) == rootString )
            {
            rootElement = currentElement;
            }
        }

    if( path.size() < 2 )
        {
        return rootElement;
        }

    if( rootElement )
        {
        return findTreeSubItem( path, 1, rootElement );
        }
    return 0;
    }

QTreeWidgetItem *explorerDock::findTreeSubItem( QStringList n, int pos, QTreeWidgetItem *p )
    {
    if( !p )
        {
        return 0;
        }
    QTreeWidgetItem *possible = 0;
    for( int x=0; x<p->childCount(); x++ )
        {
        if( p->child( x )->text( NAME_COL ) == n[pos] )
            {
            possible = p->child( x );
            if( ( n.size() - 1 ) == pos )
                {
                return possible;
                }
            else
                {
                return findTreeSubItem( n, pos+1, possible );
                }
            }
        }
    return 0;
    }
