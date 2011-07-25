#include "persistantProperties.h"
#include "QDialog"
#include "APropertyManager"
#include "QVBoxLayout"
#include "APlugin"
#include "QToolButton"
#include "QInputDialog"
#include "nodeEntity.h"
#include "ASettings"
#include "QTextEdit"
#include "QStackedWidget"
#include "helpSystem.h"
#include "gfxview.h"
#include "QPainter"
#include "QGraphicsSceneMouseEvent"

/** \class propertiesWidget
  \brief A properties Widget object is a window which contains the \p persistantProperties class.
  */

/** Create a properties widget, it automatically contains.
  */
propertiesWidget::propertiesWidget( APlugin *a, gfxview *view ) : _app( a ), _dragging( FALSE ), _resizing( FALSE )
    {
    //create the conatined widget
    _prop = new persistantProperties( a, this );

    //connect to the application signals
    connect( _prop, SIGNAL(contentsChanged()), this, SLOT(reformView()) );
    connect( view, SIGNAL(viewScaled()), this, SLOT(reformView()), Qt::UniqueConnection );
    connect( view, SIGNAL(viewMoved()), this, SLOT(reformView()), Qt::UniqueConnection );

    connect( app(), SIGNAL(cleanState()), _prop, SLOT(triggerRebuild()) );
    connect( app(), SIGNAL(openState()), _prop, SLOT(triggerRebuild()) );

    connect( app(), SIGNAL(cleanState()), this, SLOT(reformView()) );
    connect( app(), SIGNAL(openState()), this, SLOT(reformView()) );
    connect( view, SIGNAL(viewResized()), this, SLOT(reformView()) );

    // we want key input
    setFlags( ItemIsFocusable );

    // behind everything
    setZValue( -30000 );
    }

/** Toggles between being hidden and shown.
  */
void propertiesWidget::toggleHidden()
    {
    // incert the stored value
    ASettings settings( app(), "persistantProperties" );
    settings.setValue( "hidden", settings.ensureValue( "hidden", TRUE ).toBool()^true );

    // display changes
    reformView();
    }

/** Ensures the view is displayed correctly
  */
void propertiesWidget::reformView()
    {
    // make sure we are 1:1 with the view
    QTransform trans( scene()->views().front()->transform() );
    setScale( 1 / trans.m11() );

    // grab our settings from the applicaton
    ASettings settings( app(), "persistantProperties" );

    // set up hiding
    if( settings.value("hidden", true ).toBool() )
        {
        // we are dragable in the x but not y
        QPoint pt( settings.ensureValue( "position", XVector2D( 100, 100 ) ).toVector2D().toPoint() );

        pt.setY( scene()->views().front()->height() );

        setPos( scene()->views().front()->mapToScene( pt ) );

        _prop->setGeometry( QRectF( 0, 0, settings.ensureValue("width", 200).toDouble(), settings.ensureValue("height", 400).toDouble() ) );
        }
    else
        {
        // set up position in window
        setPos( scene()->views().front()->mapToScene( settings.ensureValue( "position", XVector2D( 100, 100 ) ).toVector2D().toPoint() ) );

        _prop->setGeometry( QRectF( 0, 0, settings.ensureValue("width", 200).toDouble(), settings.ensureValue("height", 400).toDouble() ) );
        }

    prepareGeometryChange();
    }

/** Called on a mouse press.
  */
void propertiesWidget::mousePressEvent( QGraphicsSceneMouseEvent *event )
    {
    // test for collision with each how point ( sizing and titlebar )
    QRectF rect( boundingRect() );
    QRectF fntRct( rect );
    fntRct.setBottom( 0 );

    QRectF bottomRight( rect.bottomRight() - QPointF( 10, 10 ), QSizeF( 10, 10 ) );
    if( bottomRight.contains( event->pos() ) )
        {
        _resizing = TRUE;
        _section = Qt::BottomRightSection;
        event->accept();
        return;
        }

    QRectF topLeft( rect.topLeft(), QSizeF( 10, 10 ) );
    if( topLeft.contains( event->pos() ) )
        {
        _resizing = TRUE;
        _section = Qt::TopLeftSection;
        event->accept();
        return;
        }

    QRectF topRight( rect.topRight() - QPointF( 10, 0 ), QSizeF( 10, 10 ) );
    if( topRight.contains( event->pos() ) )
        {
        _resizing = TRUE;
        _section = Qt::TopRightSection;
        event->accept();
        return;
        }

    QRectF bottomLeft( rect.bottomLeft() - QPointF( 0, 10 ), QSizeF( 10, 10 ) );
    if( bottomLeft.contains( event->pos() ) )
        {
        _resizing = TRUE;
        _section = Qt::BottomLeftSection;
        event->accept();
        return;
        }

    if( fntRct.contains( event->pos() ) )
        {
        _dragging = TRUE;
        event->accept();
        return;
        }

    // no collision, pass it on
    event->ignore();
    }

/** Called on a mouse move event.
  */
void propertiesWidget::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
    {
    // act appropriately, depedning on what was clicked on
    if( _dragging )
        {
        ASettings settings( app(), "persistantProperties" );

        // keep the y on the floow if were hidden
        QPoint adj( event->screenPos() - event->lastScreenPos() );
        if( settings.value("hidden").toBool() )
            {
            adj.setY( 0 );
            }
        QPoint p( settings.value( "position" ).toVector2D().toPoint() + adj );

        settings.setValue( "position", QVector2D( p ) );

        reformView();
        event->accept();
        return;
        }
    else if( _resizing )
        {
        // resize the window based on the corner grabbed
        QPointF rel( event->screenPos() - event->lastScreenPos() );

        ASettings settings( app(), "persistantProperties" );

        switch( _section )
            {
            case Qt::BottomRightSection:
                {
                settings.setValue("width", settings.value("width", 200).toDouble() + rel.x() );
                settings.setValue("height", settings.value("height", 400).toDouble() + rel.y() );
                } break;
            case Qt::TopLeftSection:
                {
                settings.setValue( "position", QVector2D( settings.value( "position" ).toVector2D().toPointF() + rel ) );
                settings.setValue("width", settings.value("width", 200).toDouble() - rel.x() );
                settings.setValue("height", settings.value("height", 400).toDouble() - rel.y() );
                } break;
            case Qt::TopRightSection:
                {
                settings.setValue( "position", QVector2D( settings.value( "position" ).toVector2D().toPointF() + QPointF( 0, rel.y() ) ) );
                settings.setValue("width", settings.value("width", 200).toDouble() + rel.x() );
                settings.setValue("height", settings.value("height", 400).toDouble() - rel.y() );
                } break;
            case Qt::BottomLeftSection:
                {
                settings.setValue( "position", QVector2D( settings.value( "position" ).toVector2D().toPointF() + QPointF( rel.x(), 0 ) ) );
                settings.setValue("width", settings.value("width", 200).toDouble() - rel.x() );
                settings.setValue("height", settings.value("height", 400).toDouble() + rel.y() );
                } break;
            default: break;
            }


        reformView();
        event->accept();
        return;
        }
    // else ignore
    event->ignore();
    }

/** Called on a double click event
  */
void propertiesWidget::mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event )
    {
    // hide if its within the title bar
    QRectF rect( boundingRect() );
    rect.setBottom( 0 );
    if( event->button() == Qt::LeftButton && rect.contains( event->pos() ) )
        {
        toggleHidden();
        }
    else
        {
        // pass it on
        QGraphicsItem::mouseDoubleClickEvent( event );
        }
    }

/** Called on a key event
  */
void propertiesWidget::keyPressEvent( QKeyEvent *event )
    {
    // hide on space bar
    if( event->key() == Qt::Key_Space )
        {
        toggleHidden();
        event->accept();
        }
    else
        {
        QGraphicsObject::keyPressEvent( event );
        }
    }

/** Called on mouse release event
  */
void propertiesWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
    {
    // cancel dragging on release
    if( _dragging || _resizing )
        {
        _dragging = FALSE;
        _resizing = FALSE;
        event->accept();
        return;
        }

    event->ignore();
    }

/** Paint the properties
  */
void propertiesWidget::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
    {
    // draw a pretty border rectangle, and a nice title
    QRectF rct( boundingRect() );

    QColor col( Qt::black );
    col.setAlpha( 128 );
    painter->setPen( Qt::transparent );
    painter->setBrush( col );
    painter->drawRoundedRect( rct, 5, 5 );

    QRectF fntRct( rct );
    fntRct.setBottom( 0 );
    fntRct.setLeft( fntRct.left() + 5 );

    painter->setPen( Qt::white );
    painter->drawText( fntRct, Qt::AlignVCenter, "Properties" );
    }

/** return the bounding rect of the properties
  */
QRectF propertiesWidget::boundingRect() const
    {
    // we build on our child widget with our border, so return this
    QRectF rect( childrenBoundingRect() );
    rect.setTop( rect.top() - 25 );
    rect.setBottom( rect.bottom() + 5 );
    rect.setLeft( rect.left() - 5 );
    rect.setRight( rect.right() + 5 );

    return rect;
    }

/** \class persistantProperties
  \brief this widget is the actual peristant properties contained within the properties widget.
  Allows addig and removing of properties.
  */

/** Create the properties with a parent and access to the plugin.
  */
persistantProperties::persistantProperties( APlugin *a, QGraphicsItem *parent ) : QGraphicsProxyWidget( parent ), _app( a ), _dragging( FALSE )
    {
    _help = 0;
    // rebuild the properties
    triggerRebuild();
    connect( app(), SIGNAL(selectionChanged()), this, SLOT(refreshSelection()) );
    }

/** Called when the application changes, to keep the help section up to date.
  */
void persistantProperties::refreshSelection()
    {
    // if help has been allocated
    if( _help )
        {
        // and something is selected
        if( app()->selection().size() )
            {
            nodeEntity *ent( app()->selection().back().castTo<nodeEntity*>() );
            if( ent )
                {
                // set up help and return
                _help->setHtml( getShortHelp( ent ) );
                return;
                }
            }
        // clear help
        _help->setHtml("");
        }
    }

/** Called to add an item to the properties widget
  */
void persistantProperties::add()
    {
    // check selection size is > 0, and a node is selected
    AEntitySearch sel( app()->selection() );
    if( sel.size() && sel.front().castTo<nodeEntity*>() )
        {
        bool ok;

        // add the properties from the selected entity to a list
        QStringList items;
        foreach( AProperty *prop, sel.front().properties() )
            {
            // no hidden or name properties
            if( !prop->hidden() && prop->name() != "entity/name" )
                {
                items << prop->displayName();
                }
            }

        // get the selection item from the user
        QString selected( QInputDialog::getItem( QApplication::activeWindow(), "Choose a property to add from " + app()->selection().front().name(), "Property", items, 0, false, &ok ) );

        // if the user clicked ok
        if( ok )
            {
            // go through the properties
            foreach( AProperty *prop, sel.front().properties() )
                {
                // if this property is the selected property
                if( selected == prop->displayName() && !prop->hidden() && prop->name() != "entity/name" )
                    {
                    // put the entity values in a variant list and put that list into the variant list
                    XVariantList newVal;

                    //stream values
                    newVal << sel.front().location().path() << prop->name();

                    // grab the current settings variant
                    ASettings settings( app(), "persistantProperties" );
                    XVariantList list( settings.value( "entities" ).toList() );

                    // add our value
                    list << XVariant( newVal );

                    // put the list back into it
                    settings.setValue( "entities", list );

                    // reset
                    triggerRebuild();
                    break;
                    }
                }
            }
        }
    else
        {
        AWarning << "No Node Selected";
        }
    }

/** Triggered to remove a property from the widget
  */
void persistantProperties::remove()
    {
    QStringList items;

    // put the properties in the settings into a list
    ASettings settings( app(), "persistantProperties" );
    XVariantList list( settings.value( "entities" ).toList() );
    foreach( XVariant var, list )
        {
        XVariantList thisEnt( var.toList() );
        if( thisEnt.size() == 2 )
            {
            // append "name" -> "property" to the list
            items << thisEnt[0].toString() + " -> " + thisEnt[1].toString();
            }
        }

    bool ok;
    // if there were actually items
    if( list.size() )
        {
        // find the user selection
        QString selected( QInputDialog::getItem( QApplication::activeWindow(), "Choose a property to remove", "Property", items, 0, false, &ok ) );
        // check for ok
        if( ok )
            {
            // search for the selected value and remove
            unsigned int x = 0;
            foreach( XVariant var, list )
                {
                XVariantList thisEnt( var.toList() );
                if( thisEnt.size() == 2 )
                    {
                    if( selected == thisEnt[0].toString() + " -> " + thisEnt[1].toString() )
                        {
                        list.removeAt( x );
                        }
                    }
                x++;
                }
            // re set the list
            settings.setValue( "entities", list );

            // rebuild the widget
            triggerRebuild();
            }
        }
    else
        {
        ALog << "No properties in palette";
        }
    }

/** Rebuild the widget when the settings widget changes, use \p triggerRebuild() instead, it calls this function.
  */
QWidget *persistantProperties::rebuild()
    {
    // allocate the widget
    QWidget *dialog( new QWidget() );

    // get layout
    QVBoxLayout *layout( new QVBoxLayout( dialog ) );
    layout->setContentsMargins( 0, 0, 0, 0 );

    // use a stacked widget to flip between help and properties
    _stack = new QStackedWidget( dialog );
    _stack->setSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored );
    layout->addWidget( _stack );

    // allocate help
    _help = new QTextEdit( _stack );
    _help->setReadOnly( TRUE );
    _stack->addWidget( _help );

    _properties = 0;
    // if the application has entities
    if( app()->search().size() )
        {
        // property manager used to create widgets
        APropertyManager mgr;

        // go through the stored list
        ASettings settings( app(), "persistantProperties" );
        XVariantList list( settings.value( "entities" ).toList() );
        foreach( XVariant var, list )
            {
            // find this entity
            XVariantList thisEnt( var.toList() );
            if( thisEnt.size() == 2 )
                {
                // search for the property spoecified in the list
                AEntitySearch search( app()->search().findEntity( thisEnt[0].toString() ) );
                QString propName( thisEnt[1].toString() );
                if( search.size() && search[0].propertyExists( propName ) )
                    {
                    // append to the property manager
                    mgr.addProperty( search[0].property( propName ) );
                    }
                else
                    {
                    ALog << "Couldn't find an entity to add its property for persistent properties palette.";
                    }
                }
            }

        // create the widget
        _properties = mgr.createWidget();
        // add it to the stack
        _stack->addWidget( _properties );
        }    //layout->addStretch();

    //set up the bottom buttons

    // layout for buttons
    QHBoxLayout *buttonLayout( new QHBoxLayout() );
    buttonLayout->setContentsMargins( 0, 0, 0, 0 );
    buttonLayout->setSpacing( 0 );
    buttonLayout->addStretch();
    layout->addLayout( buttonLayout );

    QSize iconSize( 16, 16 );

    // add button
    _add = new QToolButton( dialog );
    _add->setAutoRaise( TRUE );
    _add->setText( "Add Property" );
    _add->setIcon( QIcon( ":/calc/persistantProperties/add.png" ) );
    _add->setIconSize( iconSize );
    connect( _add, SIGNAL(clicked()), this, SLOT(add()) );

    // remove button
    _remove = new QToolButton( dialog );
    _remove->setAutoRaise( TRUE );
    _remove->setText( "Remove Property" );
    _remove->setIcon( QIcon( ":/calc/persistantProperties/remove.png" ) );
    _remove->setIconSize( iconSize );
    connect( _remove, SIGNAL(clicked()), this, SLOT(remove()) );

    // help button
    _helpToggle = new QToolButton( dialog );
    _helpToggle->setAutoRaise( TRUE );
    _helpToggle->setCheckable( TRUE );
    _helpToggle->setText( "Toggle Node Help" );
    _helpToggle->setIcon( QIcon( ":/calc/persistantProperties/help.png" ) );
    _helpToggle->setIconSize( iconSize );
    connect( _helpToggle, SIGNAL(clicked(bool)), this, SLOT(toggleHelp(bool)) );

    // add the buttons
    buttonLayout->addWidget( _remove );
    buttonLayout->addWidget( _add );
    buttonLayout->addWidget( _helpToggle );

    // set the palletes up, for transparent backgrounds on the buttons
    QPalette pal( dialog->palette() );

    pal.setColor( QPalette::Background, Qt::transparent );
    dialog->setPalette( pal );

    // reset properties palette so it displays correctly
    if( _properties )
        {
        _properties->setPalette( QApplication::palette() );
        }

    _stack->setCurrentWidget( _properties );

    return dialog;
    }

/** Used to trigger the properties widget to rebuild and set the content as itself
  */
void persistantProperties::triggerRebuild()
    {
    // delete old widget
    if( widget() )
        {
        QWidget *widg( widget() );
        setWidget( 0 );
        // set Qt to delete it later
        widg->deleteLater();
        }
    // set the widget
    setWidget( rebuild() );

    //set help up
    ASettings settings( app(), "persistantProperties" );
    toggleHelp( settings.value( "helpOn" ).toBool() );

    emit contentsChanged();
    }

/** Set the help display to \p on.
  */
void persistantProperties::toggleHelp( bool on )
    {
    // set up help display
    if( on )
        {
        _stack->setCurrentWidget( _help );
        _add->setVisible( FALSE );
        _remove->setVisible( FALSE );
        }
    else
        {
        _stack->setCurrentWidget( _properties );
        _add->setVisible( TRUE );
        _remove->setVisible( TRUE );
        }

    // store help value
    ASettings settings( app(), "persistantProperties" );
    settings.setValue( "helpOn", _stack->currentWidget() == _help );
    _helpToggle->setChecked( _stack->currentWidget() == _help );
    }

/** Used to cast this object safely with qgraphicsitem_cast<>().
  */
int persistantProperties::type() const
    {
    return Type;
    }

/** Used to cast this object safely with qgraphicsitem_cast<>().
  */
int propertiesWidget::type() const
    {
    return Type;
    }
