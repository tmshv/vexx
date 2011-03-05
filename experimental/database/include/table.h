#pragma once

#include "utility.h"
#include "field.h"

namespace db
{

class DATABASE_EXPORT table : public field
  {
public:
  //! Construct a table with only a name ( a table with no parent )
  table( );
  //! Construct a table with only a name ( a table with no parent )
  table( std::string );
  ////! Construct a table with a parent and a name
  //table( std::string, table * );
  //! Construct a table with a name which is a copy of another table ( parent is the same as the copys )
  table( std::string, const table & );
  //! Destructor
  ~table( );
  
  
  //! Child Access groups
  //! Used to get access to children of the table
  //! {
  //! Find the total number of children in the class
  unsigned int size() const;
  //! Get a child by index through the child array ( 0 for not valid )
  field *getChild( unsigned int );
  //! Get a child by name ( 0 for not valid )
  field *getChild( std::string );
  //! Get a read only child by index ( 0 for not valid )
  const field *getChild( unsigned int ) const;
  //! Get a read only child by name ( 0 for not valid )
  const field *getChild( std::string ) const;
  //! Simplified method for getting a child which is a table ( 0 for not valid )
  table *getTable( std::string );
  //! Simplified method for getting a read only child which is a table ( 0 for not valid )
  const table *getTable( std::string ) const;
  //! Get a child by name as another type ( 0 for not valid, type or child name )
  template <typename T>T *getChildAs( std::string in )
  {
    // Find the child by name, then cast it to the correct type
    return getChild( in )->getAs<T>();
  }
  //! Get a child by name as another type ( 0 for not valid, type or child name )
  template <typename T>T *getChildAs( unsigned int in )
  {
    // Get the child by index, then cast to the correct type
    return getChild( in )->getAs<T>();
  }
  //! Get a child by name as another type ( 0 for not valid, type or child name )
  template <typename T>const T *getChildAs( std::string in ) const
  {
    // Find the child by name, then cast it to the correct type
    return getChild( in )->getAs<T>();
  }
  //! Get a child by name as another type ( 0 for not valid, type or child name )
  template <typename T>const T *getChildAs( unsigned int in ) const
  {
    // Get the child by index, then cast to the correct type
    return getChild( in )->getAs<T>();
  }
  //! }
  
  //! Child Management Methods
  //! Used to add and remove children of the table
  //! {
  //! Remove and delete all children
  void clear();
  //! Add a field as a child ( will remove the child from the old parent )
  void addChild( field * );
  //! Add a field as a child ( will remove the child from the old parent )
  void addChild( field & );
  //! Remove a specific child by pointer
  void deleteChild( field * );
  //! Remove a specific child by index
  void deleteChild( unsigned int );
  //! Remove a specific child by pointer THE OBJECT IS NOT DELETED
  void removeChild( field * );
  //! Remove a specific child by index THE OBJECT IS NOT DELETED
  void removeChild( unsigned int );
  //! }
  
  //! Find if this table currently has a child by the name given
  bool nameIsUnique( std::string );
  //! Copy this table and return a pointer to it
  virtual field *copy( table *tab ) const;
  //! Set this field form another field, without reallocating any data, for assignment
  virtual void setFromField( const field * );
protected:
  //! Enter this table into a binary stream containing it, and all its children
  virtual void getDataAsStream( binaryStream & ) const;
  //! Set this table from a binary stream which should contain a table
  virtual void setDataFromStream( binaryStream & );
private:
  //! Private normal copy operator as you cant have a copy with the same name as its copyee
  table( const table & );
  //! vector of children
  std::vector <field *> _children;
  std::vector <bool> _childrenDelete;
  };
  
std::ostream& DATABASE_EXPORT operator<<( std::ostream&, const table& );
}
