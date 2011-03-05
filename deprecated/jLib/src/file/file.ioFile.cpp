#include "file/file.ioFile.h"

BEGIN_JLIB_FILE_NAMESPACE

ioFile::ioFile( string _n )
    {
    name = _n;
    }

ioFile::ioFile( const ioFile &in )
    {
    name = in.name;
    }

void ioFile::setFilename( string _n )
    {
    name = _n;
    }

string ioFile::getFilename( ) const
    {
    return name;
    }

string &ioFile::get( )
    {
    if( buffer == "" )
        {
        reload();
        }

    return buffer;
    }

std::fstream &ioFile::operator()( )
    {
    return reference;
    }

void ioFile::reload( )
    {
    buffer = "";
    if( exists() )
        {

        open();

        while( !reference.eof() )
            {
            char x;
            reference.read( &x, 1 );
            buffer += x;
            }

        buffer.erase( buffer.size() - 1 );

        reference.seekg( 0, std::ios::beg );
        }
    }

void ioFile::write( )
    {
    if( !reference.is_open() )
        {
        reference.open( name.c_str(), std::ios::binary | std::ios::trunc | std::fstream::in | std::fstream::out );
        }
    if( reference.is_open() && reference.good() )
        {
        seek( 0 );
        reference<<buffer;
        }
    }

void ioFile::seek( std::streampos _x )
    {
    if( reference.is_open() )
        {
        reference.seekg( _x );
        }
    }

void ioFile::seek( std::streamoff _o, std::ios_base::seekdir _d )
    {
    if( reference.is_open() )
        {
        reference.seekg( _o, _d );
        }
    }

unsigned int ioFile::tell( )
    {
    if( reference.is_open() )
        {
        return reference.tellg( );
        }
    return 0;
    }

bool ioFile::exists( )
    {
    std::fstream temp;
    temp.open( name.c_str(), std::fstream::in );
    return temp.is_open();
    }

void ioFile::set( string _x )
    {
    buffer = _x;
    }

unsigned int ioFile::length()
    {
    return buffer.length();
    }

void ioFile::open()
    {
    reference.open( name.c_str(), std::ios::binary | std::fstream::in | std::fstream::out );
    }

void ioFile::close()
    {
    reference.close();
    }

ioFile::~ioFile( )
    {
    close();
    }

bool ioFile::exists( string name )
    {
    std::fstream temp;
    temp.open( name.c_str(), std::fstream::in );
    return temp.is_open();
    }

bool ioFile::remove( string name )
    {
    jFixme<<"Remove the file "<<name<<std::endl;
    return 0;
    }

string ioFile::get( string x )
    {
    ioFile temp( x );
    return temp.get();
    }

bool ioFile::remove( )
    {
    jFixme<<"Remove the file "<<name<<endl;
    return 0;
    }

END_JLIB_FILE_NAMESPACE
