#include "colourPickerWidget.h"
#include "QHBoxLayout"
#include "QToolButton"
#include "APropertyManager"
#include "ALog"
#include "viewerDock.h"

colourPickerWidget::colourPickerWidget( AProperty *in ) : _property( in )
    {
    // layout for the property
    QHBoxLayout *layout( new QHBoxLayout( this ) );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 2 );

    // greate a standard widget for the property
    QWidget *col( 0 );
    if( in->type() == calcPropertyInfo::Colour )
        {
        col = APropertyManager::createWidget( *in, propertyInfo::Colour );
        }
    else
        {
        col = APropertyManager::createWidget( *in, propertyInfo::ColourNoAlpha );
        }

    // add a button and the original property to the layout
    layout->addWidget( col );

    // button should be at the top of the larger widget
    QVBoxLayout *buttonLayout( new QVBoxLayout( ) );
    layout->addLayout( buttonLayout );
    _button = new QToolButton( );
    _button->setCheckable( true );
    _button->setIcon( QIcon( ":/calc/colourPicker.svg" ) );
    buttonLayout->addWidget( _button );

    // stretch keeps the button at the top
    buttonLayout->addStretch();

    // register for colour picked signals
    viewerDock::registerColourPicker( this );
    }

colourPickerWidget::~colourPickerWidget( )
    {
    // on deletion we remove ourselves from the viewer dock.
    viewerDock::removeColourPicker( this );
    }

QWidget *colourPickerWidget::create( AProperty *prop )
    {
    // called to get a new property
    return new colourPickerWidget( prop );
    }

void colourPickerWidget::colourPicked( XColour colour, bool isFinished )
    {
    // if the button is currently checked
    if( _button->isChecked() )
        {
        // set the property
        *_property = colour;
        if( isFinished )
            {
            // if the selecting is finished, then uncheck the button
            _button->setChecked( false );
            }
        }
    }
