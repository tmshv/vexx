#include "viewport.h"
#include "viewportEntity.h"

viewport::viewport( QWidget *parent, APlugin *a ) : viewportWidgetBase( parent, jLib::math::colour4( 0.7 ), TRUE ), _requestRefresh( FALSE ),
        _background( root.passP() )
    {
    _app = a;
    manipulators.setScene( root );

    root.camera().translate = jLib::math::triple( 10, 5.5, 10 );
    root.camera().direction = jLib::math::triple( 0, 2.5, 0 );
    root.camera().changed();

    connect( app(), SIGNAL(selectionChanged()), this, SLOT(selectionChanged()) );
    connect( app(), SIGNAL(entityAdded(AEntity*)), this, SLOT(entityAdded(AEntity*)) );
    connect( app(), SIGNAL(entityRemoved(AEntity*)), this, SLOT(entityRemoved(AEntity*)) );
    }

viewport::~viewport()
    {
    clearSelectionBoxes();
    }

void viewport::entityAdded( AEntity *ent )
    {
    if( ent->propertyExists( "viewport/object" ) )
        {
        connect( ent, SIGNAL(propertyChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)) );
        _entities << static_cast <viewportEntity *> (ent);
        refreshHeirachy( );
        }
    }

void viewport::entityRemoved( AEntity *ent )
    {
    if( ent->propertyExists( "viewport/object" ) )
        {
        viewportEntity *vEnt( static_cast <viewportEntity *> (ent) );
        if( _entities.contains( vEnt ) )
            {
            disconnect( ent, SIGNAL(propertyChanged(APropertyDevice*,AProperty*)), this, SLOT(entityChanged(APropertyDevice*,AProperty*)) );
            _entities.remove( vEnt );
            refreshHeirachy();
            }
        }
    }

void viewport::entityChanged( APropertyDevice *, AProperty * )
    {
    refreshHeirachy( );
    }

void viewport::refreshHeirachy( )
    {
    root.pass().clear();
    _background.setParent( root.passP() );

    FOREACH( _tools, t )
        {
        if( (*t)->object() )
            {
            (*t)->object()->setParent( root.passP() );
            }
        }

    AEntitySearch rootChildren = app()->search().find3DEntities.findDirectChildren( AEntityLocation( "/" ) );
    for( int x=0; x<rootChildren.size(); x++ )
        {
        jAssert( dynamic_cast <viewportEntity *>( &(rootChildren[x]) ) );
        jLib::graphics::entity::entityD *obj = static_cast <viewportEntity *>( &(rootChildren[x]) )->object();
        if( obj )
            {
            obj->setParent( root.passP() );
            }
        }

    update();
    }

void viewport::geometryChanged()
    {
    _geometryChanged = TRUE;
    }

void viewport::refreshBBoxes()
    {
    clearSelectionBoxes();

    AEntitySearch drawables = app()->selection().find3DEntities;

    for( int x=0; x<drawables.size(); x++ )
        {
        viewportEntity *vE = static_cast <viewportEntity *> (&(drawables[x]));
        _boxes.push_back( new boundingBox( vE->object()->parent(), vE->bounds().minimum(), vE->bounds().maximum() ) );
        }
    update();
    }

void viewport::selectionChanged()
    {
    refreshBBoxes();
    }

void viewport::update()
    {
    _requestRefresh = TRUE;
    }

void viewport::paintGL()
    {
    emit contextActivate();
    viewportWidgetBase::paintGL();
    }

void viewport::refresh()
    {
    if( _geometryChanged )
        {
        _geometryChanged = FALSE;
        refreshBBoxes();
        _requestRefresh = TRUE;
        }
    if( root.camera().changed() || _requestRefresh )
        {
        redraw();
        _requestRefresh = FALSE;
        }
    }

void viewport::clearSelectionBoxes()
    {
    FOREACH( _boxes, b )
        {
        delete *b;
        }
    _boxes.clear();
    }

void viewport::addTool( viewportTool *in )
    {
    _tools.insert( in );
    refreshHeirachy( );
    }

void viewport::removeTool( viewportTool *in )
    {
    _tools.remove( in );
    refreshHeirachy( );
    }
