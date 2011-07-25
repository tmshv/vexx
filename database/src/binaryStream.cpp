#include "binaryStream.h"
#include <iostream>
#include <stdio.h>

namespace db
{
void binaryStream::resize( unsigned int s )
{
  _data.resize( s );
}
char *binaryStream::getDataPtr()
{
  return &(_data[0]);
}

const char *binaryStream::getDataPtr() const
{
  return &(_data.front());
}

bool binaryStream::isValid()
{
  return _data.size() != 0;
}
void binaryStream::writeOut( std::string name )
{
  FILE *fp;
  fp = fopen( name.c_str(), "w" );

  if( _data.size() > 0 )
  {
    if( fp != 0 )
    {
      fwrite( &(_data[0]), 1, _data.size(), fp );
      fclose(fp);
    }
    else
    {
      std::cout<<"Error writing file, because you are a fool."<<std::endl;
    }
  }
  else
  {
    std::cout<<"Error writing file, buffer has a length of zero."<<std::endl;
  }

}

void binaryStream::readIn( std::string name )
{
  FILE *fp;
  fp = fopen( name.c_str(), "r" );

  if( fp != 0 )
  {
    fseek( fp, 0, SEEK_END );
    long si = ftell( fp );

    _data.resize( si );
    
    if( si > 0 )
    {
      fseek( fp, 0, SEEK_SET );
      fread( &(_data[0]), 1, si, fp );
    }
    else
    {
      std::cout<<"Error reading file, it has a length of zero."<<std::endl;
    }
    
    fclose(fp);
  }
  else
  {
    std::cout<<"Error reading file, does it exist?"<<std::endl;
  }
}

void binaryStream::pushChar( const char in )
{
  _data.push_back( in );
}
  
void binaryStream::pushSize( const size_t in )
{
  pushType( in );
}
  
void binaryStream::pushString( const std::string in )
{
  pushCharStream( in.c_str(), in.length() );
}

void binaryStream::pushCharStream( const char *data, const size_t size )
{
  pushSize( size );
  for( size_t x=0; x<size; x++ )
  {
    pushChar( data[x] );
  }
}
char binaryStream::popChar( )
{
  char val = _data.front();
  _data.erase( _data.begin() );
  return val;
}
  
size_t binaryStream::popSize( )
{
  return popType<size_t>();
}

std::string binaryStream::popString( )
{
  std::vector<char> stream = popCharStream();
  std::string temp;
  for( unsigned int x=0; x<stream.size(); x++ )
  {
    temp += stream[x];
  }
  return temp;
}

std::vector<char> binaryStream::popCharStream( )
{
  std::vector<char> ret;
  size_t size = popSize( );
  for( size_t x=0; x<size; x++ )
    {
    ret.push_back( popChar( ) );
    }
  return ret;
}
}
