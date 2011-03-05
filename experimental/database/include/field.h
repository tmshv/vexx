#pragma once

#include "utility.h"
#include "binaryStream.h"

namespace db
{
class table;

class DATABASE_EXPORT field
{
public:
  //! Construct by name, no parent
  field( std::string );
  ////! Construct by name with a parent
  //field( std::string, table & );
  //! Copy a field
  field( const field & );
  //! Destroy the field
  virtual ~field();
  
  //! Set the fields name
  void setName( std::string );
  //! Get the fields name
  std::string getName( ) const
    { return name(); }
  //! Get the fields name
  std::string name( ) const;
  //! find the fields parent
  table *getParent() const
    { return parent(); }
  table *parent() const;
  //! Change the fields parent
  void setParent( table * );
  //! Cast this field to some type
  template <typename T>T *getAs()
  {
    assert( typeid( T ) == typeid( *this ) );
    return static_cast <T *> (this); // returns this field object, dynamically cast as T
  }
  template <typename T>const T *getAs() const
  {
    assert( typeid( T ) == typeid( *this ) );
    return static_cast <const T *> (this); // returns this field object, dynamically cast as T
  }
  
  //! Copy this field and return a pointer to this copy
  virtual field *copy( table * ) const = 0;
  //! Set this field form another field, without reallocating any data, for assignment
  virtual void setFromField( const field * );

  //! Binary compilation functions
  //! Used to compress the data in the field to binary
  //! {
  binaryStream getAsStream(  ) const;
  void getAsStream( binaryStream & ) const;
  void setFromStreamRef( binaryStream & );
  void setFromStream( binaryStream );
  //! }
protected:
  virtual void getDataAsStream( binaryStream & ) const = 0;
  virtual void setDataFromStream( binaryStream & ) = 0;
  std::string generateUniqueName( field *in )
  {
    std::ostringstream stream;
    stream<<"FIELD_"<<in;
    return stream.str();
  }
private:
  //! Name of the field
  std::string _name;
  //! Parent of the field ( may be 0 )
  table *_parent;
};
}
