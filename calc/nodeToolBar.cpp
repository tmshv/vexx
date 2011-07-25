#include "nodeToolBar.h"
#include "QSettings"
#include "QAction"
#include "nodeDB.h"
#include "nodeEntity.h"
#include "QPainter"
#include "ALog"

#define iconSize QSize( 24, 24 )

nodeToolBar::nodeToolBar()
    {
    // connect to my signals
    connect( this, SIGNAL(actionTriggered(QAction*)), this, SLOT(create(QAction*)) );

    setIconSize( iconSize );

    nodeDB::nodeTypesWatcher *watcher( new nodeDB::nodeTypesWatcher( this ) );
    connect( watcher, SIGNAL(typesChanged()), this, SLOT(rebuild()) );
    }

void nodeToolBar::added()
    {
    rebuild();
    }

void nodeToolBar::create( QAction* act )
    {
    // create a node and insert it
    nodeDB::nodeType type( act->data() );
    type.createAndInsert( app() );
    }

void  nodeToolBar::rebuild()
    {
    clear();

    // find groups available
    QStringList groups( nodeDB::nodeGroups() );

    foreach( QString group, groups )
        {
        // find the types available
        foreach( nodeDB::nodeType t, nodeDB::listNodeGroup(group) )
            {
            if( !t.icon().isNull() )
                {
                // create an image with a correct node colour
                QPixmap pix( t.icon().pixmap(iconSize) );

                QImage icon( iconSize, QImage::Format_ARGB32_Premultiplied );
                icon.fill( qRgba(0,0,0,0) );

                QPainter ptr( &icon );
                ptr.setRenderHint( QPainter::Antialiasing, true );
                ptr.setPen( Qt::black );
                ptr.setBrush( t.colour() );
                ptr.drawRoundedRect( icon.rect(), 5, 5 );
                ptr.drawPixmap( 0, 0, pix );

                // add an action
                QAction *act( addAction( QPixmap::fromImage( icon ), t.name() ) );
                // serialise type and store it.
                act->setData( t );
                }
            else
                {
                // add an action
                QAction *act( addAction( t.name() ) );
                // serialise type and store it.
                act->setData( t );
                }
            }
        // separate for different groups
        addSeparator();
        }
    }
