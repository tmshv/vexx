#include "APrivatePropertyWidgetFactory.h"
#include "../SDK/AProperty"

#include "APrivateGuiProperties.h"

static QHash <int, QPair<guiPropertyTypeFunction,guiPropertyValidatorFunction> > _customTypes;

/** \cond INTERNAL
  */

/** \class APrivatePropertyWidgetFactory
  \brief Manages creation of GUI controls for properties.
  */

/** Create a widget for the property \p property.
  */
QWidget *APrivatePropertyWidgetFactory::widgetFor( AProperty *property )
    {
    return widgetFor( property, property->type() );
    }

/** \overload
  Create a widget for the property \p property, with type \p forceType.
  */
QWidget *APrivatePropertyWidgetFactory::widgetFor( AProperty *property, int forceType )
    {
    // custom types take priority
    if( _customTypes.contains( forceType ) )
        {
        return _customTypes[forceType].first( property );
        }
    // otherwise try the default type
    switch( forceType )
        {
        case propertyInfo::Bool: return new aGP::APrivateBoolProperty( property );
        case propertyInfo::Int: return new aGP::APrivateIntProperty( property );
        case propertyInfo::Double: return new aGP::APrivateFloatProperty( property );
        case propertyInfo::Colour: return new aGP::APrivateColourProperty( property, TRUE );
        case propertyInfo::ColourNoAlpha: return new aGP::APrivateColourProperty( property, FALSE );
        case propertyInfo::Component2D:
            return new aGP::APrivateVector2DProperty( property );
        case propertyInfo::Direction:
        case propertyInfo::Position:
        case propertyInfo::Scale:
        case propertyInfo::Vector:
        case propertyInfo::Component3D:
            return new aGP::APrivateVector3DProperty( property );
        case propertyInfo::Component4D:
            return new aGP::APrivateVector4DProperty( property );
        case propertyInfo::File: return new aGP::APrivateFileProperty( property );
        case propertyInfo::Directory: return new aGP::APrivateDirectoryProperty( property );
        case propertyInfo::LongString: return new aGP::APrivateLongStringProperty( property );
        case propertyInfo::FileSequence: return new aGP::APrivateFileSequenceProperty( property );
        default: return new aGP::APrivateStringProperty( property );
        }
    }

/** Register a custom GUI type for a proeprty of type \p in with creator function \p type,
  and validator function \p val.
  \note If val if 0, no validator is used.
  */
void APrivatePropertyWidgetFactory::registerType( int in, guiPropertyTypeFunction type, guiPropertyValidatorFunction val )
    {
    // add to static list
    _customTypes[in] = QPair<guiPropertyTypeFunction,guiPropertyValidatorFunction>( type, val );
    }

/** Remove the type from the DB
  */
void APrivatePropertyWidgetFactory::unregisterType( int in  )
    {
    _customTypes.remove( in );
    }

/** Try to validate the type with a custom validator.
  */
bool APrivatePropertyWidgetFactory::tryCustomValidator( int in, AVariant &var )
    {
    if( _customTypes.contains(in) && _customTypes[in].second )
        {
        return _customTypes[in].second( var );
        }
    return TRUE;
    }

/** \endcond
  */
