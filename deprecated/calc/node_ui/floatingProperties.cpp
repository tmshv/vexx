#include "floatingProperties.h"
#include "QGraphicsProxyWidget"
#include "node.h"
#include "QGraphicsView"
#include "QPainter"
#include "APropertyManager"
#include "nodegraphicaldetails.h"
#include "QStackedWidget"
#include "exportWidget.h"
#include "QTabWidget"
#include "scriptEdit.h"
#include "helpSystem.h"
#include "QPushButton"
#include "QVBoxLayout"
#include "nodeEditDialog.h"
#include "groupEditDialog.h"
#include "QGraphicsSceneMouseEvent"
#include "groupNode.h"
#include "ALog"

/** \class floatingProperties
  \brief A floating properties object is a graphical item which controls an entity's settings
  */

/** Create a floating properties object, with parent \p parent.
  This item will control the entity \p parent->getEntity().
  */
floatingProperties::floatingProperties( nodeGraphicalDetails *parent ) : QGraphicsObject( parent ),
        _parent( parent ), _controller( parent->getEntity() ), _renderExporter( 0 )
    {
    // we go behind our parent for drawing
    setFlags( ItemStacksBehindParent );

    // set our proxy so we can render normal widgets inside ourself
    _proxy = new QGraphicsProxyWidget( this );
    _proxy->setPos( 0, 5 );

    // our widge stack the tab controls
    _stack = new QStackedWidget();
    _proxy->setWidget( _stack );

    // set our stack to be transparent
    QPalette pal( _stack->palette() );
    pal.setBrush( QPalette::Background, Qt::transparent );
    _stack->setPalette( pal );

    // create the "properties" stack item
    _widget = APropertyManager::createWidget( parent->getEntity() );

    // create the "Develop" tab
    _source = new QWidget();
    QVBoxLayout *layout( new QVBoxLayout( _source ) );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 2 );

    if( _controller->type() != groupNode::identifier() )
        {
        // add a script editor
        _script = new scriptEdit( _source );
        layout->addWidget( _script );

        // set up the text
        _script->setText( _controller->property("nodeEntity/source")->toString() );
        _script->setReadOnly( !_controller->property("editable")->toBool());

        // add a button for further controls if the node is editable
        if( _controller->property("editable")->toBool() )
            {
            QHBoxLayout *subLayout( new QHBoxLayout() );
            QPushButton *button( new QPushButton( "More Node Controls", _source ) );
            connect( button, SIGNAL(clicked()), this, SLOT(editNode()) );
            subLayout->addStretch();
            subLayout->addWidget( button );
            layout->addLayout( subLayout );
            }
        }
    else
        {
        // add a script editor
        _script = new scriptEdit( _source );
        layout->addWidget( _script );
        _script->setText( "// This is a group node, it has no source.\n// Use Ctrl + Double Click to enter and edit the node." );
        _script->setReadOnly( true );

        // add a button for further controls if the node is a group
        QHBoxLayout *subLayout( new QHBoxLayout() );

        QPushButton *button( new QPushButton( "Group Controls", _source ) );
        connect( button, SIGNAL(clicked()), this, SLOT(editGroup()) );

        QPushButton *enter( new QPushButton( "Enter Group", _source ) );
        connect( enter, SIGNAL(clicked()), this, SLOT(enterGroup()), Qt::QueuedConnection );

        subLayout->addStretch();
        subLayout->addWidget( button );
        subLayout->addWidget( enter );

        layout->addLayout( subLayout );
        }


    // help tab
    _help = new QTextEdit( _stack );
    _help->setHtml( getShortHelp( _controller ) );
    _help->setReadOnly( TRUE );

    // connect the entity to this
    connect( _script, SIGNAL(textChanged()), this, SLOT(sourceChanged()) );
    connect( _controller, SIGNAL(isViewNodeChanged()), this, SLOT(reloadWidget()), Qt::UniqueConnection );
    connect( _controller, SIGNAL(entityChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)), Qt::UniqueConnection );
    connect( scene()->views().front(), SIGNAL(viewScaled()), this, SLOT(viewScaled()), Qt::UniqueConnection );
    connect( scene()->views().front(), SIGNAL(viewScaled()), this, SLOT(viewScaled()), Qt::UniqueConnection );

    // add the tabs to this
    _tabData << tabStruct( "Properties", _widget );
    _tabData << tabStruct( "Developer", _source );
    _tabData << tabStruct( "Help", _help );

    // set up the tabs
    calculateTabs();

    // ensure we display correctly
    reloadWidget();
    viewScaled();

    // set the index to the last saved index
    _stack->setCurrentIndex( _controller->property("propertiesState")->toInt() );
    }

/** Destroy the properties
  */
floatingProperties::~floatingProperties( )
    {
    }

/** Recalculate the display data for the tabs, should be called every time a tab is added or removed.
  */
void floatingProperties::calculateTabs()
    {
    // find the current tab to redisplay after were reconfigured
    QWidget *widg( _stack->currentWidget() );
    QMap <QString, float> widths;

    // grab the application font well use to draw
    QFont fnt;
    QFontMetrics fntMet( fnt );

    // calculate the width of the texts for each tab
    int fullWidth=0;
    foreach( tabStruct tab, _tabData )
        {
        // tab is font width + 10 pixels padding
        int tempWidth = fntMet.width( tab.text ) + 10;
        widths.insert( tab.text, tempWidth );
        fullWidth += tempWidth;

        // ensure the widgets are in the stacked widget
        _stack->addWidget( tab.widget );
        tab.widget->setPalette( QApplication::palette() );
        }

    // if this is over the limit
    // squash the text in
    float sub( ( (float)fullWidth - 360.0 ) / _tabData.size() );

    // ensure each tab is reduced or expanded to fit the full width
    foreach( QString str, widths.keys() )
        {
        widths[str] = widths[str] - sub;
        }

    // calculate the new text and draw rectangles for the tabs
    float left = 0;
    int x=0;
    foreach( tabStruct tab, _tabData )
        {
        float tempWidth( widths[tab.text] );

        tab.rect = QRectF( left, 0, tempWidth, 35 );
        left += tempWidth;

        tab.calculatedText = fntMet.elidedText( tab.text, Qt::ElideRight, tempWidth - 10 );

        _tabData[x] = tab;
        x++;
        }

    // try to restore old widget
    if( widg != 0 )
        {
        _stack->setCurrentWidget( widg );
        }
    }

/** Paint the properties.
  */
void floatingProperties::paint( QPainter *painter, const QStyleOptionGraphicsItem *, QWidget * )
    {
    // format the height of the stacked widgets
    formatHeight();

    // draw the tabs

    // grab the base colour
    QColor col( _parent->colour() );
    // translate to the base tab position
    painter->translate( -5, _height );
    // current tab index for tab highlighting
    int currentIndex( _stack->currentIndex() );

    // draw each tab
    foreach( tabStruct tab, _tabData )
        {
        // set up the brush and pen
        int add( 0 );
        painter->setPen( Qt::transparent );
        if( _stack->currentWidget() == tab.widget )
            {
            add = 5;
            painter->setBrush( col );
            }
        else
            {
            // we set the non selected tab colour to an lighter and darker colour values centred around the selected tab
            painter->setBrush( col.darker( 100 + -20 * ( currentIndex - _stack->indexOf( tab.widget ) ) ) );
            }
        // draw the tab and text
        painter->drawRoundedRect( tab.rect.left(), tab.rect.top(), tab.rect.width()+1, tab.rect.height()+add, 8, 8 );
        painter->setPen( textColour() );
        painter->drawText( tab.rect.left()+5, tab.rect.top(), tab.rect.width()-10, tab.rect.height()+add-3, Qt::AlignHCenter|Qt::AlignBottom, tab.calculatedText );
        }
    // reset the painter position
    painter->translate( 5, -_height );

    // draw a nice border for the stacked widget
    painter->setPen( Qt::transparent );
    painter->setBrush( _parent->colour() );
    painter->drawRoundedRect( QRectF( -5, -20, 360, _height + 30 ), 5, 5 );
    }

/** The bounding rect of the properties
  */
QRectF floatingProperties::boundingRect() const
    {
    return QRectF( -5, -20, 360, _height + 60 );
    }

/** Set and ensure the heights of the widgets contained conform with the size of the properties border and tabs
  */
void floatingProperties::formatHeight()
    {
    // height default
    int hint( 250 );
    // find the recommended height
    if( _stack->currentWidget() )
        {
        hint =_stack->currentWidget()->sizeHint().height();
        }

    // resize all children to fit.
    _height = qMin( hint, 500 );
    _stack->resize( 350, _height );
    _widget->resize( 350, _height );

    // update if height changed
    if( _oldHeight != _height )
        {
        prepareGeometryChange();
        scene()->update();
        }

    // store old height
    _oldHeight = _height;
    }

/** Return the type of this object, used for safe casting with qgraphicsitem_cast<XXX>()
  */
int floatingProperties::type() const
    {
    return Type;
    }

/** Slot triggered when the view scale changes. Used to keep the floating properties a constant size
  */
void floatingProperties::viewScaled()
    {
    // make sure we are 1:1 with the view
    QTransform trans( scene()->views().front()->transform() );
    // set our scale
    setScale( 1 / sqrt( trans.m11()*trans.m11() + trans.m21() + trans.m21() ) );
    // set our position
    _proxy->setPos( 0, 5 );
    // tell the scene were changing size
    prepareGeometryChange();
    }

/** Slot triggered to begin node editing, and pop up the node edit dialog.
  */
void floatingProperties::editNode()
    {
    // create the dialog amd show it
    nodeEditDialog *dia( new nodeEditDialog( _parent->app(), _controller, 0 ) );
    connect( dia, SIGNAL(entityChanged()), scene(), SIGNAL(refreshScene()) );
    connect( dia, SIGNAL(finished(int)), dia, SLOT(deleteLater()) );
    dia->show();
    }

/** Enter the group for editing
  */
void floatingProperties::enterGroup()
    {
    _controller->array()->setWorkingPath( _controller->location() );
    }

/** Trigger the edit group dialog
  */
void floatingProperties::editGroup()
    {
    // create the dialog amd show it
    groupEditDialog *dia( new groupEditDialog( _parent->app(), _controller, 0 ) );
    connect( dia, SIGNAL(entityChanged()), scene(), SIGNAL(refreshScene()) );
    connect( dia, SIGNAL(finished(int)), dia, SLOT(deleteLater()) );
    dia->show();
    }

/** Reload the widget, controlling the display of the export tab, and showing the widgets correctly
  */
void floatingProperties::reloadWidget()
    {
    // display or hide the view node

    // if this node is the view node
    if( _parent->getEntity()->viewNode() == 1 )
        {
        // if we dont have an export widget
        if( !_renderExporter )
            {
            // make one and attach it
            _renderExporter = new exportWidget( _parent->getEntity() );
            _tabData << tabStruct( "Export", _renderExporter );
            }
        }
    else // otherwise ensure there is no export tab
        {
        // if there is an export widget
        if( _renderExporter )
            {
            // check each tab
            for( int x=0; x<_tabData.size(); x++ )
                {
                if( _tabData[x].widget == _renderExporter )
                    {
                    _tabData.removeAt( x );
                    break;
                    }
                }
            delete _renderExporter;
            _renderExporter = 0;
            }
        }
    // set up the tabs, and ensure the stacks height
    calculateTabs();
    formatHeight();
    }

/** Returns a contrasting colour to the background colour ( _parent->colour() ). Returns White or Black.
  */
QColor floatingProperties::textColour() const
    {
    // return colour for the lightness
    if( _parent->colour().lightness() > 100 )
        {
        return QColor( Qt::black );
        }
    return QColor( Qt::white );
    }

/** Triggered when the Source changes in the developer tab, used to set the property.
  */
void floatingProperties::sourceChanged()
    {
    _settingSource = true;
    _controller->property("nodeEntity/source") = _script->toPlainText();
    _settingSource = false;
    }

/** Return the display mode of the properties.
  */
floatingProperties::displayMode floatingProperties::mode() const
    {
    return displayMode(_stack->currentIndex());
    }

/** Set the display mode of the properties.
  */
void floatingProperties::setMode( displayMode in )
    {
    // cache the old mode
    displayMode oldMode( mode() );

    // set the index
    _stack->setCurrentIndex( in );

    // check if the index has changed, and reload if it has
    if( mode() != oldMode )
        {
        _controller->property("propertiesState") = mode();
        reloadWidget();
        }
    }

/** Called on a mouse move event.
  */
void floatingProperties::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
    {
    // If we receive a mouse move event, it means we received and accepted a mouse press. so just absorb this event
    event->accept();
    }

/** Called when the entity this properties is attached to changes to update the proprties
  */
void floatingProperties::entityChanged( APropertyDevice*, AProperty *prop )
    {
    // set the text of the source
    if( prop->name() == "nodeEntity/source" && !_settingSource )
        {
        _script->setText( prop->value().toString() );
        }
    if( prop->name() == "shortHelp" )
        {
        // set the short help
        _help->setHtml( prop->value().toString() );
        }
    }

/** Called on mouse press event.
  */
void floatingProperties::mousePressEvent( QGraphicsSceneMouseEvent *event )
    {
    // check if the mouse clicked on a tab
    QPointF pos( event->pos() - QPointF( -5, _height ) );
    foreach( tabStruct tab, _tabData )
        {
        if( tab.rect.contains( pos ) )
            {
            // set as the current tab if it was clicked on
            _stack->setCurrentWidget( tab.widget );
            _controller->property("propertiesState") = _stack->currentIndex();
            event->accept();
            }
        }

    event->ignore();
    }

/** Called on mouse release event.
  */
void floatingProperties::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
    {
    event->accept();
    }
