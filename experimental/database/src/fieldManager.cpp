#include "fieldManager.h"
#include "iostream"

namespace db
{
void fieldManager::ensureRegisteredType( field *in, std::string nN )
{
  if( typeid( *in ) != typeid( field ) )
  {
    if( !isRegistered( in ) )
    {
      fieldRecord temp;
      temp.type = in->copy( 0 );
      temp.niceName = nN;
      types.push_back( temp );
    }
  }
}

bool fieldManager::isRegistered( field *in )
{
  for( unsigned int x=0; x<types.size(); x++ )
  {
    if( typeid( *types[x].type ) == typeid( *in ) )
    {
      return true;
    }
  }
  return false;
}

field *fieldManager::getField( std::string name )
{
  field *orig = getOriginal( name );
  if( orig )
  {
    return reinterpret_cast<field *>(orig->copy( 0 ));
  }
  return 0;
}
std::string fieldManager::getName( field *in )
{
  for( unsigned int x=0; x<types.size(); x++ )
  {
    if( typeid( *(types[x].type) ) == typeid( *in ) && types[x].niceName != "" )
    {
      return types[x].niceName;
    }
  }
  return typeid( *in ).name();
}

field *fieldManager::getOriginal( std::string in )
{
  for( unsigned int x=0; x<types.size(); x++ )
  {
    if( typeid( *(types[x].type) ).name() == in || types[x].niceName == in )
    {
      return types[x].type;
    }
  }
  return 0;
}

fieldManager *fieldManager::instance( bool force )
{
  static bool beenCalled = false;
  static fieldManager *me;
  if( !beenCalled || force )
  {
    beenCalled = true;
    me = new fieldManager();
  }
  return me;
}

void fieldManager::empty()
{
  delete instance();
}

fieldManager::fieldManager()
{
}
fieldManager::fieldManager( const fieldManager & )
{
}
fieldManager::~fieldManager()
{
  for( unsigned int x=0; x<types.size(); x++ )
  {
    delete types[x].type;
  }
}
}
