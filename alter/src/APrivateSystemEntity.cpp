#include "APrivateSystemEntity.h"

/** \cond INTERNAL
  */

/** \class APrivateSystemEntity
  \brief APrivateSystemEntity is used to record the type of program arrays were generated with, and ensure versioning is correct.
  */

/** Create a APrivateSystemEntity
  */
APrivateSystemEntity::APrivateSystemEntity( )
    {
    addProperty( "system/program", "" );
    addProperty( "system/domain", "" );
    addProperty( "system/organisation", "" );
    addProperty( "system/version", "" );
    hide();
    }

/** Set the data of the systemEntity
  */
void APrivateSystemEntity::setData( QString program, QString domain, QString organisation, QString version )
    {
    // set properties
    property( "system/program" ) = program;
    property( "system/domain" ) = domain;
    property( "system/organisation" ) = organisation;
    property( "system/version" ) = version;
    }

/** \endcond
  */
