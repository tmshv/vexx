#include "../SDK/AEntityType"
#include "QCryptographicHash"
#include "../SDK/AEntityLocation"
#include "QVariant"

/** \class AEntityType
  \brief Represents the type of entity. Allows querying of this type.
  */

/** \fn AEntityType::XProperty( QString, name, setName )
  Holds and sets of the name of the type.
  */

/** \fn AEntityType::XProperty( QString, group, setGroup )
  Holds and sets of the group of the type.
  */

/** \fn AEntityType::XROProperty( QByteArray, hash )
  A read only property for the hash of a type.
  */

/** Create an type from a name \p n, and the owner \p o.
  */
AEntityType::AEntityType( QString o, QString n )
    {
    AEntityLocation fullName( n );
    // the name is only one directory level
    setName( fullName.name() );
    // the group is the rest of the levels
    setGroup( fullName.directory().replace("/","") );
    // the hash is o and n combined for a unique string
    _hash = QCryptographicHash::hash( ( o + "/" + n ).toAscii(), QCryptographicHash::Sha1 );
    }

/** Create a type from a QVariant
  */
AEntityType::AEntityType( QVariant var )
    {
    QVariantList list( var.toList() );
    // check the vairant has enough data to be a type
    if( list.size() == 3 )
        {
        // extract data from the variant
        _name = list[0].toString();
        _group = list[1].toString();
        _hash = list[2].toByteArray();
        }
    else
        {
        _name = "NULL";
        }
    }

/** Compress the type to a variant.
  */
AEntityType::operator QVariant()
    {
    // add all data
    return QVariantList() << _name << _group << _hash;
    }

/** Check for equality between two types
  \todo Use hash here?
  */
bool AEntityType::operator ==( const AEntityType &in ) const
    {
    // check for name equality
    return name() == in.name();
    }

/** Check for inequality between two types
  \todo Use hash here?
  */
bool AEntityType::operator !=( const AEntityType &in ) const
    {
    // check for name inequality
    return name() != in.name();
    }
