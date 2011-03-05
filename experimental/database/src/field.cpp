#include "field.h"
#include "table.h"
#include "fieldManager.h"

namespace db
{

field::field( std::string n ) : _name( n ), _parent( 0 ) // if constructed without a parent, set _parent to 0
{
}

  
/*field::field( std::string n, table &p ) : _name( n ), _parent( &p )
{
  if(_parent)
  {
    _parent->addChild( this );
  }
}*/

field::field( const field &in ) : _name( in._name ), _parent( in._parent ) // if constructing using another field, copy the same attributes
{
}

field::~field()
{
  if(_parent)
  {
    _parent->removeChild( this );
  }
}

void field::setName( std::string in )
{
  _name = in;
}

std::string field::name( ) const
{
  return _name;
}

table *field::parent() const
{
  return _parent;
}

void field::setParent( table *in )
{
  assert( in != this );
  if( _parent != 0 )
  {
    _parent->removeChild( this );
  }
  _parent = in;
  if( _parent != 0 )
  {
    _parent->addChild( this );
  }
}

binaryStream field::getAsStream(  ) const
{
  binaryStream stream;
  stream.pushString( getName() );
  getDataAsStream( stream );
  return stream;
}
void field::getAsStream( binaryStream &stream ) const
{
  stream.pushString( getName() );
  getDataAsStream( stream );
}

void field::setFromStream( binaryStream in )
  {
  binaryStream conv( in );
  setFromStream( conv );
  }

void field::setFromStreamRef( binaryStream &stream )
{
  if( stream.isValid() )
  {
    setName( stream.popString( ) );
    setDataFromStream( stream );
  }
}

void field::setFromField( const field *fi )
  {
  binaryStream stream;
  fi->getDataAsStream( stream );
  setDataFromStream( stream );
  }
}
