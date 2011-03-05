#pragma once

#include "utility.h"
#include "field.h"
#include "sstream"

namespace db
{

template <typename T> class dataField : public field // Create a template class datafield that inherits from field
  {
  public:
    //// Datafield constructor.  Field has no null constructor so pass name into field constructor
    /*dataField( std::string name, T in, table &tab ) : field( name, tab )
    {
      _data = in;
    }*/
    // Datafield constructor.  Field has no null constructor so pass name into field constructor
    dataField( std::string name, T in ) : field( name ) 
    {
      _data = in;
    }
    // Datafield constructor.  Field has no null constructor so pass name into field constructor
    dataField( std::string name ) : field( name )
    {
    }
    // Datafield constructor. Unnamed field, uses a unique name automatically
    dataField( ) : field( generateUniqueName( this ) )
    {
    }
    // construct from copy ( with a different name or name clashing occurs )
    /*dataField( std::string name, table &tab ) : field( name, tab )
    {
    }*/

    // Allows construction of datafields using other datafields of the same type.  Field can be constructed using a reference to another field
    // The datafield "in" is automatically cast to a field and passed into the constructor for a new field
    dataField( const dataField<T> &in ) : field( in ) 
    {
      _data = in();
    }
    //overloading '='
    void operator=( T in )
    {
      _data = in;
    }
    void set( T in )
    {
      _data = in;
    }
    T get()
    {
      return _data;
    }
    const T get() const
    {
      return _data;
    }
    T operator()()
    {
      return _data;
    }
    const T operator()() const
    {
      return _data;
    }
    virtual field *copy( table *tab ) const
    {
      field *ret = new dataField<T>( getName(), get() );
      if( tab )
      {
        tab->addChild( ret );
      }
      return ret;
    }
  protected:
    virtual void getDataAsStream( binaryStream &stream ) const
    {
      stream.pushType( _data );
    }
    virtual void setDataFromStream( binaryStream &stream )
    {
      _data = stream.popType<T>();    
    }
  protected:
    T _data;
  };

#define TYPEDEF_DATAFIELD( name, type ) typedef db::dataField<type> name;

TYPEDEF_DATAFIELD( intField, int )
TYPEDEF_DATAFIELD( floatField, float )
TYPEDEF_DATAFIELD( charField, char )
TYPEDEF_DATAFIELD( boolField, bool )

class stringField : public dataField<std::string>
{
public:
  //stringField( std::string name, std::string in, table &tab ) : dataField<std::string>( name, in, tab ) { }
  stringField( std::string name, std::string in ) : dataField<std::string>( name, in ) { }
  stringField( std::string name ) : dataField<std::string>( name ) { }
  //stringField( std::string name, table &tab ) : dataField<std::string>( name, tab ) { }
  stringField( const stringField &in ) : dataField<std::string>( in ) { }
  virtual field *copy( table *tab ) const
  {
    field *ret = new stringField( getName(), get() );
    if( tab )
    {
      tab->addChild( ret );
    }
    return ret;
  }
protected:
  void getDataAsStream( binaryStream &stream ) const
  {
    stream.pushString( _data );
  }
  void setDataFromStream( binaryStream &stream )
  {
    _data = stream.popString( );
  }
};
}
