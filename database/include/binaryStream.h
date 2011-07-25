#pragma once

#include "vector"
#include "string"
#include "utility.h"

namespace db
{
class DATABASE_EXPORT binaryStream
  {
  public:
  bool isValid();
  void writeOut( std::string );
  void readIn( std::string );
  void pushChar( const char );
  template <typename T>void pushType( const T in )
    {
    const char *temp = reinterpret_cast <const char *> (&in);
    for( size_t x=0; x<sizeof( T ); x++ )
      {
      pushChar( temp[x] );
      }
    }
  void pushSize( const size_t );
  void pushString( const std::string );
  void pushCharStream( const char *, const size_t );

  char popChar( );
  template <typename T>T popType( )
    {
    T ret;
    char *temp = reinterpret_cast <char *> (&ret);
    for( size_t x=0; x<sizeof( T ); x++ )
      {
      temp[x] = popChar( );
      }
    return ret;
    }
  size_t popSize( );
  std::string popString( );
  std::vector<char> popCharStream( );
  
  void clear()
    {
    _data.clear();
    }
  unsigned int size() const
    {
    return _data.size();
    }
  char data( unsigned int x )
    {
    return _data[x];
    }
  char data( unsigned int x ) const
    {
    return _data[x];
    }
  void resize( unsigned int );
  char *getDataPtr();
  const char *getDataPtr( ) const;
private:
  std::vector <char> _data;
  friend class networkSocket;
  };
}
