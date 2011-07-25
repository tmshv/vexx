#pragma once

#include "utility.h"
#include "field.h"
#include <typeinfo>

namespace db
{

class DATABASE_EXPORT fieldManager
{
public:
  //! Destructor
  ~fieldManager();
  //! Ensuer the type given is registered ( will not register field as a type )
  void ensureRegisteredType( field *, std::string="" );
  //! chekc is a type is registered
  bool isRegistered( field * );
  //! get a field of the type given
  field *getField( std::string );
  //! get the string representation of the field type given
  std::string getName( field * );

  //! get the singleton
  static fieldManager *instance( bool = false );
  //! delete the singleton
  static void empty();
  
private:
  struct fieldRecord
  {
    field *type;
    std::string niceName;
  };
  //! Access the original field copy by name
  field *getOriginal( std::string );
  //! Array of types catalogued
  std::vector <fieldRecord> types;
  //! Constructor default
  fieldManager();
  //! Copy, not used, but not public
  fieldManager( const fieldManager & );
};

#define DECLARE_FIELD_TYPE( cl, na ) {                                    \
  cl tempDecl__MCRO( "tempDecl" );                                        \
  db::fieldManager::instance()->ensureRegisteredType( &tempDecl__MCRO, na );   \
}

}
