#include "../SDK/AProperty"
#include "APrivatePropertyCommand.h"
#include "APrivateAttributeCommand.h"
#include "../SDK/ALog"
#include "APrivateCommandTools.h"

#include "APrivatePropertyWidgetFactory.h"

#include "QFile"

/* A Range of validators, which cast to a property type, then return whether the cast was possible
   */
namespace validator
    {
    bool toBool( AVariant &in )
        {
        in = in.toBool();
        return TRUE;
        }
    bool toInt( AVariant &in )
        {
        in = in.toInt();
        return TRUE;
        }
    bool toDouble( AVariant &in )
        {
        in = in.toDouble();
        return TRUE;
        }
    bool toVector2D( AVariant &in )
        {
        in = in.toVector2D();
        return TRUE;
        }
    bool toVector3D( AVariant &in )
        {
        in = in.toVector3D();
        return TRUE;
        }
    bool toVector4D( AVariant &in )
        {
        in = in.toVector4D();
        return TRUE;
        }
    bool toColour( AVariant &in )
        {
        in = in.toColour();
        return TRUE;
        }
    bool toColourNoAlpha( AVariant &in )
        {
        XColour tmp( in.toColour() );
        tmp.setA( 1.0 );
        in = tmp;
        return TRUE;
        }
    bool toQuaternion( AVariant &in )
        {
        in = in.toQuaternion();
        return TRUE;
        }
    };

int AProperty::_nextID = 0;

/** \class AAttribute
  \brief An attribute in a property. An attribute has an AVariant value and can be used to control the way a property behaves.
  */

/** \fn AAttribute::AAttribute( AProperty *parent, QString name );
  Create an attribute with parent \p parent and name \p name.
  */

/** \fn AAttribute::operator=( const AVariant &in )
  Assign this attributa new variant value
  */

/** \fn AAttribute::operator AVariant()
  Use this attribute as the variant is holds.
  */

/** \fn AAttribute::operator->()
  Access the attribute as its variant value.
  */

/** \class AConstAttribute
  \brief An attribute in a property. An attribute has an AVariant value and can be used to control the way a property behaves.
  */


/** \fn AConstAttribute::AConstAttribute( AProperty *parent, QString name );
  Create an attribute with parent \p parent and name \p name.
  */

/** \fn AConstAttribute::operator AVariant()
  Use this attribute as the variant is holds.
  */

/** \fn AConstAttribute::operator->()
  Access the attribute as its variant value.
  */

/** \class AProperty
  \brief An AProperty class is a property belonging to an APropertyDevice. It holds its own value,
  and an array of attributes, defining and controlling its use.
  */

/** \cond INTERNAL
  \fn AProperty::onPropertyChange(AProperty*)
  \brief Emitted when the property changes.
  \endcond
  */

/** \cond INTERNAL
  \fn AProperty::onCommand(command**)
  \brief Emitted so the device can catch and pass on commands from the property.
  \endcond
  */

/** Create an AProperty from its parent \p device, the name of the property \p name, the initial value \p val
  the type of the property \p t, and the display name \p dName.
  */
AProperty::AProperty( APropertyDevice *device, QString name, const AVariant &val, int t, QString dName )
        : _device( device ), _name( name ), _informing( FALSE )
    {
    _attributes["type"] = t;
    _attributes["displayName"] = dName;
    _value = val;
    // think this is deprecated
    _thisID = _nextID++;
    }

/** Return the name of the property.
  */
QString AProperty::name() const
    {
    return _name;
    }

/** Return the map of attributes belonging to this property.
  */
const AProperty::attributeMap &AProperty::attributes() const
    {
    return _attributes;
    }

/** Get a reference to the specific attribute \p name.
  \note This function will add attributes that dont exist automatically
  */
AAttribute AProperty::attribute( QString name )
    {
    return AAttribute( this, name );
    }

/** Remove an attribute.
  \todo undo for this action
  */
void AProperty::removeAttribute( QString in )
    {
    emitCommand( APrivateAttributeCommand( _device, _name, in, _attributes[in], AVariant() ) )
    }

/** Return a const reference to the attribute \p name.
  \note This function won't add a new attribute automatically. You should test it exists first.
  */
AConstAttribute AProperty::attribute( QString name ) const
    {
    return AConstAttribute( this, name );
    }

/** Triggers a change of the attribute \p attribName to \p in.
  */
void AProperty::changeAttribute( QString attribName, const AVariant &in )
    {
    // we emit a command to the undo stack, this takes care of the operation
    emitCommand( APrivateAttributeCommand( _device, _name, attribName, _attributes[attribName], in ) )
    }

/** Find the type of this property. This function returns an integer usually used to determine
  what kind of Gui control should be displayed for this type.
  */
int AProperty::type() const
    {
    // find the attribute and return it
    return attributes().find( "type" ).value().toInt();
    }

/** Set the type of this property
  */
void AProperty::setType( int in )
    {
    // set the type
    attribute( "type" ) = in;
    }

/** Assign this property a new value
  */
AProperty &AProperty::operator =( const AVariant &in )
    {
    // store the new value so we can edit it
    AVariant temp( in );
    bool valid = FALSE;

    // test for the type it claims to be, and assign it ot this type.
    // if valid returns false we can also fail the change completely
    switch( type() )
        {
        case propertyInfo::Bool:
            {
            valid = validator::toBool( temp );
            } break;

        case propertyInfo::Int:
            {
            valid = validator::toInt( temp );
            } break;

        case propertyInfo::Float:
        case propertyInfo::Double:
        case propertyInfo::Time:
            {
            valid = validator::toDouble( temp );
            } break;

        case propertyInfo::Component2D:
            {
            valid = validator::toVector2D( temp );
            } break;

        case propertyInfo::Position:
        case propertyInfo::Scale:
        case propertyInfo::Direction:
        case propertyInfo::Vector:
        case propertyInfo::Component3D:
            {
            valid = validator::toVector3D( temp );
            } break;

        case propertyInfo::Component4D:
            {
            valid = validator::toVector4D( temp );
            } break;

        case propertyInfo::Orientation:
            {
            valid = validator::toQuaternion( temp );
            } break;

        case propertyInfo::Colour:
            {
            valid = validator::toColour( temp );
            } break;

        case propertyInfo::ColourNoAlpha:
            {
            valid = validator::toColourNoAlpha( temp );
            } break;

        case propertyInfo::File:
        case propertyInfo::Directory:
            {
            valid = true;
            } break;

        case propertyInfo::String:
        default:
            {
            // we can also try to use a custom validator defined by the programmer on it
            valid = APrivatePropertyWidgetFactory::tryCustomValidator( type(), temp );
            } break;
        }
    // actually change the property if the change was valid
    if( valid )
        {
        emitCommand( APrivatePropertyCommand( _device, _name, _value, temp ) );
        }
    return *this;
    }

/** Return the property's value.
  */
AProperty::operator AVariant() const
    {
    return _value;
    }

/** Find the propertys value for quick use. eg. property->toDouble() rather than property.value().toDouble()
  */
const AVariant *AProperty::operator ->( ) const
    {
    return &( _value );
    }

/** Return the value of the property.
  */
const AVariant &AProperty::value() const
    {
    return _value;
    }

/** Returns whether the property wants to be hidden
  */
bool AProperty::hidden() const
    {
    return _attributes.find("hide") != _attributes.end();
    }

/** Sets the property's hide attribute to \p able
  */
void AProperty::hide( bool able )
    {
    if( able )
        {
        attribute("hide") = TRUE;
        }
    else
        {
        attribute("hide") = AVariant();
        }
    }

/** Get the property's display name
  */
QString AProperty::displayName() const
    {
    // search for the attribute, and return "" if the attribute doesnt exist
    AConstAttribute da( attribute( "displayName" ) );
    if( !da->isNull() )
        {
        return da->toString();
        }
    return "";
    }

/** Set the propertys display name to \p in
  */
void AProperty::displayName( const QString &in )
    {
    attribute("displayName") = in;
    }

/** Return the propertys parent device
  */
APropertyDevice *AProperty::parent( )
    {
    return _device;
    }

