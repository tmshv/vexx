#include "colourPickerWidget.h"
#include "QHBoxLayout"
#include "QToolButton"
#include "APropertyManager"
#include "ALog"
#include "viewerDock.h"

colourPickerWidget::colourPickerWidget( AProperty *in ) : _property( in )
    {
    QHBoxLayout *layout( new QHBoxLayout( this ) );
    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 2 );

    QWidget *col( 0 );
    if( in->type() == calcPropertyInfo::Colour )
        {
        col = APropertyManager::createWidget( *in, propertyInfo::Colour );
        }
    else
        {
        col = APropertyManager::createWidget( *in, propertyInfo::ColourNoAlpha );
        }
    layout->addWidget( col );

    QVBoxLayout *buttonLayout( new QVBoxLayout( ) );
    layout->addLayout( buttonLayout );
    _button = new QToolButton( );
    _button->setCheckable( true );
    _button->setIcon( QIcon( ":/calc/colourPicker.svg" ) );
    buttonLayout->addWidget( _button );

    buttonLayout->addStretch();

    viewerDock::registerColourPicker( this );
    }

colourPickerWidget::~colourPickerWidget( )
    {
    viewerDock::removeColourPicker( this );
    }

QWidget *colourPickerWidget::create( AProperty *prop )
    {
    return new colourPickerWidget( prop );
    }

void colourPickerWidget::colourPicked( XColour colour, bool isFinished )
    {
    if( _button->isChecked() )
        {
        *_property = colour;
        if( isFinished )
            {
            _button->setChecked( false );
            }
        }
    }
