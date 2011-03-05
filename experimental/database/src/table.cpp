#include "table.h"
#include "fieldManager.h"
#include <iostream>
#include <sstream>
#include "assert.h"
#include "dataField.h"

namespace db
{

table::table( ) : field( generateUniqueName( this ) )
  {
  }

table::table( std::string name ) : field( name )
  {
  }
  
/*table::table( std::string name, table *tab ) : field( name, *tab )
  {
  }*/

table::table( std::string name, const table &cp ) : field( name )
  {
  if( cp.getParent() )
    {
    cp.getParent()->addChild( this );
    }
  for( unsigned int x=0; x<cp.size(); x++ )
    {
    cp.getChild(x)->copy( this );
    }
  }

table::table( const table & ) : field( "NULL_COPY" )
  {
  assert( 0 );
  }
  
table::~table( )
  {
  clear();
  }

unsigned int table::size() const
  {
  return _children.size();
  }

field *table::getChild( unsigned int x )
{
  return _children[x];
}

field *table::getChild( std::string in )
{
  for( unsigned int x=0; x<_children.size(); x++ )
  {
    if( _children[x]->getName() == in )
    {
      return _children[x];
    }
  }
  return 0;
}

const field *table::getChild( unsigned int x ) const
{
  return _children[x];
}

table *table::getTable( std::string name )
{
  return dynamic_cast <table *> ( getChild( name ) );
}

const table *table::getTable( std::string name ) const
{
  return dynamic_cast <const table *> ( getChild( name ) );
}

const field *table::getChild( std::string in ) const
{
  for( unsigned int x=0; x<_children.size(); x++ )
  {
    if( _children[x]->getName() == in )
    {
      return _children[x];
    }
  }
  return 0;
}
  
void table::addChild( field *ref )
  {
  assert( nameIsUnique( ref->getName() ) );
  _children.push_back( ref );
  _childrenDelete.push_back( true );
  fieldManager::instance()->ensureRegisteredType( ref );
  }

void table::addChild( field &ref )
  {
  assert( nameIsUnique( ref.getName() ) );
  _children.push_back( &ref );
  _childrenDelete.push_back( false );
  fieldManager::instance()->ensureRegisteredType( &ref );
  }

void table::removeChild( field *del )
{
  for( unsigned int x=0; x<_children.size(); x++ )
  {
    if( _children[x] == del )
    {
      removeChild( x );
    }
  }
}

void table::deleteChild( unsigned int x )
{
  assert( x < size() );
  if( _childrenDelete[x] )
  {
    delete _children[x];
  }
  _children.erase( _children.begin() + x );
  _childrenDelete.erase( _childrenDelete.begin() + x );
}  

void table::deleteChild( field *del )
{
  for( unsigned int x=0; x<_children.size(); x++ )
  {
    if( _children[x] == del )
    {
      deleteChild( x );
    }
  }
}

void table::removeChild( unsigned int x )
{
  assert( x < size() );
  _children.erase( _children.begin() + x );
}
  
bool table::nameIsUnique( std::string in )
  {
  for( unsigned int x=0; x<_children.size(); x++ )
    {
    if( _children[x]->getName() == in )
      {
      return false;
      }
    }
  return true;
  }  

void table::setFromField( const field *fi )
  {
  setName( fi->getName() );
  assert( static_cast <const table *>(fi) );
  const table *ta = static_cast <const table *>(fi);
  for( unsigned int x=0; x<_children.size(); x++ )
    {
    field *thisFi = getChild( x );\
    const field *curFi = ta->getChild( thisFi->getName() );
    if( curFi )
      {
      assert( typeid( *thisFi ) == typeid( *curFi ) );
      thisFi->setFromField( curFi );
      }
    }
  }

field *table::copy( table *tab ) const
  {
  table *ret = new table( getName() );
  if( tab )
  {
    tab->addChild( ret );
  }


  for( unsigned int x=0; x<size(); x++ )
  {
    reinterpret_cast <field *> ( getChild(x)->copy( ret ) );
  }
  return ret;
  }
  
/*std::string table::getDebugString( ) const
  {
  static int tab = 0;
  std::string tabs;
  for( unsigned int x=0; x<tab; x++ )
    {
    tabs += "| ";
    }
  std::ostringstream ret;
  ret << "+ " + getName() + "\n" << tabs << "|\n"; ;
  tab++;
  std::string retDebug, name;
  for( unsigned int x=0; x<_children.size(); x++ )
    {
    retDebug = _children[x]->getDebugString();
    name = _children[x]->getName();
    if( dynamic_cast<table *>( _children[x] ) )
      {
      ret << tabs << ">-" << retDebug;
      ret << tabs << "|\n";
      }
    else
      {
      ret << tabs << ">-+ " << name << "\n";
      ret << tabs << "| >- " << retDebug << "\n";
      ret << tabs << "|\n";
      }
    }
  tab--;
  
  return ret.str();
  
  }*/

void table::clear()
  {
  for( unsigned int x=0; x<_children.size(); x++ )
  {
    deleteChild( x );
  }
  _children.clear();
  _childrenDelete.clear();
  }
  
void table::getDataAsStream( binaryStream &stream ) const
  {
  stream.pushSize( _children.size() );
  for( unsigned int x=0; x<_children.size(); x++ )
  {
    stream.pushString( fieldManager::instance()->getName( _children[x] ) );
    _children[x]->getAsStream( stream );
  }
  }

void table::setDataFromStream( binaryStream &stream )
  {
  clear();
  size_t num = stream.popSize();
  for( unsigned int x=0; x<num; x++ )
    {
    _children.push_back( fieldManager::instance()->getField( stream.popString() ) );
    _childrenDelete.push_back( true );
    _children.back()->setFromStreamRef( stream );
    }
  }

std::ostream& operator<<( std::ostream& os, const table& s )
  {
  static unsigned int tabs = 0;
  std::string tabStr;
  for( unsigned int x=0; x<tabs; x++ )
    {
    tabStr += " |";
    }
  std::string firstTabStr = tabStr;
  (*(--firstTabStr.end())) = '+';
  os<<firstTabStr<<"-+ table "<<s.getName()<<std::endl;
  for( unsigned int x=0; x<s.size(); x++ )
    {
    const field *curr = s.getChild( x );
    if( typeid( *curr ) == typeid( table ) )
      {
      tabs++;
      os<<(*static_cast<const table *>(curr));
      tabs--;
      }
    else
    {
      os<<tabStr<<" +- ";
      if( dynamic_cast<const dataField<float> *>( curr ) )
        {
        os<<"float "<<curr->getName()<<" = "<<dynamic_cast<const dataField<float> *>( curr )->get();
        }
      else if( dynamic_cast<const stringField *>( curr ) )
        {
        os<<"string "<<curr->getName()<<" = \""<<dynamic_cast<const stringField *>( curr )->get()<<"\"";
        }
      else if( dynamic_cast<const dataField<int> *>( curr ) )
        {
        os<<"int "<<curr->getName()<<" = "<<dynamic_cast<const dataField<int> *>( curr )->get();
        }
      else if( dynamic_cast<const dataField<bool> *>( curr ) )
        {
        os<<"bool "<<curr->getName()<<" = "<<dynamic_cast<const dataField<bool> *>( curr )->get();
        }
      else if( dynamic_cast<const dataField<char> *>( curr ) )
        {
        os<<"char "<<curr->getName()<<" = "<<dynamic_cast<const dataField<char> *>( curr )->get();
        }
      else
        {
        os<<"Unknown "<<curr->getName()<<" '"<<typeid( *curr ).name()<<"'";
        }
      os<<std::endl;
    }
    }
  return os;
  }
}
