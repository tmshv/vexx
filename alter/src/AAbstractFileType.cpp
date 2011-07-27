#include "../SDK/AAbstractFileType"

/** \struct AAbstractFileType::entity
  \brief This structure holds a temporary data list that represents an entity. \p type holds a unique type identifier for the entity type, and \p properties contains the properties for the entity held in name <-> data pairs.
  */

/** \struct AAbstractFileType::property
  \brief This structure holds a temporary representation of a property, \p value holds the value of the property, and \p attributes holds the attributes for the property.
  */

/** \class AAbstractFileType
  \brief This class is an abstract interface to implement a new file type.
  */

/** \fn AAbstractFileType::XProperty( QString, name, setName )
  Gives access to the file types name.
  */

/** \fn AAbstractFileType::XRefProperty( QList <QString>, extensions )
  Gives access to the extension list.
  */

/** \fn AAbstractFileType::createFileContents( ) const
  This function should be implemented to create a string containing the contents of the entities() array.
  */

/** \fn AAbstractFileType::parseFileContents( const QString & )
  This function should be implemented to fill the entities() array from the contents held in the string.
  */

/** Creates an AAbstractFileType with the name \p name
  \sa setName()
  */
AAbstractFileType::AAbstractFileType( QString name )
    {
    setName( name );
    }

/** Destory the file type
  */
AAbstractFileType::~AAbstractFileType()
    {
    }

/** Returns a filter string that can be used with the AFileDialog class. The return is similar to "File type name ( *.extA *.extB *.extC )"
  */
QString AAbstractFileType::getFilterString() const
    {
    QString ret;
    // cant be valid if there are no extensions or no name
    if( name() != "" && extensions().size() && extensions()[0] != "" )
        {
        ret += name() + " (";

        // append each extension
        for( int y=0; y<extensions().size(); y++ )
            {
            ret += "*." + extensions()[y];
            if( y < ( extensions().size() - 1 ) )
                {
                ret += " ";
                }
            }
        ret += ")";
        }
    return ret;
    }

/** Returns a default suffix for the type, returns the first non empty extension
  */
QString AAbstractFileType::getDefaultSuffix() const
    {
    // return the first extension which is valid
    for( int y=0; y<extensions().size(); y++ )
        {
        if( extensions()[y] != "" && extensions()[y] != "*" )
            {
            return extensions()[y];
            }
        }
    return "*";
    }

/** Clears this file types internal entity register so it can be filled again, or load from a new file
  */
void AAbstractFileType::clear()
    {
    _entities.clear();
    }

/** Returns this file types internal file hash
  */
QHash <QString, AAbstractFileType::entity> &AAbstractFileType::entities()
    {
    return _entities;
    }

/** \overload
  */
const QHash <QString, AAbstractFileType::entity> &AAbstractFileType::entities() const
    {
    return _entities;
    }
