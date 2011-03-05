#include "jLib.variant.h"

BEGIN_JLIB_NAMESPACE

variant::variant() : _type( Null ), _int( 0 )
    {
    }

variant::variant( const variant &in ) : _type( Null )
    {
    fromVariant( in );
    }

variant::~variant()
    {
    clear();
    }

bool variant::isNull() const
    {
    return _type == Null;
    }

void variant::clear()
    {
    if( _type == Null ||
        _type == String ||
        _type == Bool ||
        _type == Int ||
        _type == UnsignedInt ||
        _type == Char ||
        _type == Double ||
        _type == VectorX ||
        _type == QtVariant )
        {
        if( _type == VectorX )
            {
            delete reinterpret_cast <math::vectorX *> ( _other );
            }
        else if( _type == String )
            {
            delete reinterpret_cast <string *> ( _other );
            }
        else if( _type == QtVariant )
            {
            delete reinterpret_cast <QVariant *> ( _other );
            }
        }
    else
        {
        jError<<"Variant not cleaned effectively "<<_type<<endl;
        }
    _type = Null;
    _int = 0;
    }

variant &variant::operator =( const variant &in )
    {
    fromVariant( in );
    return *this;
    }

void variant::fromVariant( const variant &in )
    {
    clear();
    if( in._type == String )
        {
        fromString( in.toString() );
        }
    else if( in._type ==  Bool )
        {
        fromBool( in.toBool() );
        }
    else if( in._type == Int )
        {
        fromInt( in.toInt() );
        }
    else if( in._type == UnsignedInt )
        {
        fromUnsignedInt( in.toUnsignedInt() );
        }
    else if( in._type == Char )
        {
        fromChar( in.toChar() );
        }
    else if( in._type == Double )
        {
        fromDouble( in.toDouble() );
        }
    else if( in._type == VectorX )
        {
        fromVectorX( in.toVectorX() );
        }
    else if( in._type == QtVariant )
        {
        fromQtVariant( in.toQtVariant() );
        }
    }

void variant::fromCharString( const char *in )
    {
    fromString( in );
    }

const char *variant::toCharString( ) const
    {
    return toString().c_str();
    }

void variant::fromString( const string &in )
    {
    clear();
    _other = new string( in );
    _type = String;
    }

string variant::toString( ) const
    {
    if( _type == String )
        {
        return *reinterpret_cast <string *> ( _other );
        }
    else if( _type ==  Bool )
        {
        std::ostringstream stream;
        stream<<_bool;
        return stream.str();
        }
    else if( _type == Int )
        {
        std::ostringstream stream;
        stream<<_int;
        return stream.str();
        }
    else if( _type == UnsignedInt )
        {
        std::ostringstream stream;
        stream<<_uint;
        return stream.str();
        }
    else if( _type == Char )
        {
        std::ostringstream stream;
        stream<<_char;
        return stream.str();
        }
    else if( _type == Double )
        {
        std::ostringstream stream;
        stream<<_double;
        return stream.str();
        }
    else if( _type == VectorX )
        {
        std::ostringstream stream;
        stream<<*reinterpret_cast <math::vectorX *> (_other);
        return stream.str();
        }
    else if( _type == QtVariant )
        {
        return reinterpret_cast <QVariant *> ( _other )->toString().toStdString();
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        jDebug<<"this should be looked at"<<endl;
        return reinterpret_cast <list <variant> *> (_other)->front().toString();
        }
    return "";
    }

void variant::fromBool( bool in )
    {
    clear();
    _bool = in;
    _type = Bool;
    }

bool variant::toBool( ) const
    {
    if( _type == String )
        {
        std::istringstream stream( *reinterpret_cast <string *> (_other) );
        bool to;
        stream>>to;
        return to;
        }
    else if( _type ==  Bool )
        {
        return _bool;
        }
    else if( _type == Int )
        {
        return _int;
        }
    else if( _type == UnsignedInt )
        {
        return _uint;
        }
    else if( _type == Char )
        {
        return _char;
        }
    else if( _type == Double )
        {
        return _double;
        }
    else if( _type == VectorX )
        {

        return ( reinterpret_cast <math::vectorX *> (_other) == 0 );
        }
    else if( _type == QtVariant )
        {
        return reinterpret_cast <QVariant *> ( _other )->toBool();
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        return reinterpret_cast <list <variant> *> (_other)->front().toBool();
        }
    return FALSE;
    }

void variant::fromInt( int in )
    {
    clear();
    _int = in;
    _type = Int;
    }

int variant::toInt( ) const
    {
    if( _type == String )
        {
        std::istringstream stream( *reinterpret_cast <string *> (_other) );
        int to;
        stream>>to;
        return to;
        }
    else if( _type ==  Bool )
        {
        return _bool;
        }
    else if( _type == Int )
        {
        return _int;
        }
    else if( _type == UnsignedInt )
        {
        return _int;
        }
    else if( _type == Char )
        {
        return _char;
        }
    else if( _type == Double )
        {
        return (int)_double;
        }
    else if( _type == VectorX )
        {
        return (int)(*reinterpret_cast <math::vectorX *> (_other))[0];
        }
    else if( _type == QtVariant )
        {
        return reinterpret_cast <QVariant *> ( _other )->toInt();
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        return reinterpret_cast <list <variant> *> (_other)->front().toInt();
        }
    return 0;
    }

void variant::fromUnsignedInt( unsigned int in )
    {
    clear();
    _uint = in;
    _type = UnsignedInt;
    }

unsigned int variant::toUnsignedInt( ) const
    {
    if( _type == String )
        {
        std::istringstream stream( *reinterpret_cast <string *> (_other) );
        unsigned int to;
        stream>>to;
        return to;
        }
    else if( _type ==  Bool )
        {
        return _bool;
        }
    else if( _type == Int )
        {
        return _int;
        }
    else if( _type == UnsignedInt )
        {
        return _uint;
        }
    else if( _type == Char )
        {
        return _char;
        }
    else if( _type == Double )
        {
        return (unsigned int )_double;
        }
    else if( _type == VectorX )
        {
        return (unsigned int )(*reinterpret_cast <math::vectorX *> (_other))[0];
        }
    else if( _type == QtVariant )
        {
        return reinterpret_cast <QVariant *> ( _other )->toInt();
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        return reinterpret_cast <list <variant> *> (_other)->front().toUnsignedInt();
        }
    return 0;
    }

void variant::fromChar( char in )
    {
    clear();
    _char = in;
    _type = Char;
    }

char variant::toChar( ) const
    {
    if( _type == String )
        {
        std::istringstream stream( *reinterpret_cast <string *> (_other) );
        char to;
        stream>>to;
        return to;
        }
    else if( _type ==  Bool )
        {
        return _bool;
        }
    else if( _type == Int )
        {
        return _int;
        }
    else if( _type == UnsignedInt )
        {
        return _uint;
        }
    else if( _type == Char )
        {
        return _char;
        }
    else if( _type == Double )
        {
        return (char)_double;
        }
    else if( _type == VectorX )
        {
        return (char)(*reinterpret_cast <math::vectorX *> (_other))[0];
        }
    else if( _type == QtVariant )
        {
        return reinterpret_cast <QVariant *> ( _other )->toChar().toAscii();
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        return reinterpret_cast <list <variant> *> (_other)->front().toChar();
        }
    return 0;
    }

void variant::fromDouble( double in )
    {
    clear();
    _double = in;
    _type = Double;
    }

double variant::toDouble( ) const
    {
    if( _type == String )
        {
        std::istringstream stream( *reinterpret_cast <string *> (_other) );
        double to;
        stream>>to;
        return to;
        }
    else if( _type ==  Bool )
        {
        return _bool;
        }
    else if( _type == Int )
        {
        return _int;
        }
    else if( _type == UnsignedInt )
        {
        return _uint;
        }
    else if( _type == Char )
        {
        return _char;
        }
    else if( _type == Double )
        {
        return _double;
        }
    else if( _type == VectorX )
        {
        return (*reinterpret_cast <math::vectorX *> (_other))[0];
        }
    else if( _type == QtVariant )
        {
        return reinterpret_cast <QVariant *> ( _other )->toDouble();
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        return reinterpret_cast <list <variant> *> (_other)->front().toDouble();
        }
    return 0.0;
    }

void variant::fromTriple( const math::triple &in )
    {
    fromVectorX( in );
    }

math::triple variant::toTriple( ) const
    {
    math::vectorX ret( toVectorX() );
    if( ret.size() >= 3 )
        {
        return math::triple( ret[0], ret[1], ret[2] );
        }
    else if( ret.size() == 2 )
        {
        return math::triple( ret[0], ret[1], 0.0 );
        }
    else if( ret.size() == 1 )
        {
        return math::triple( ret[0], 0.0, 0.0 );
        }
    return math::triple( 0.0 );
    }

void variant::fromColour( const math::colour4 &in )
    {
    fromVectorX( in );
    }

math::colour4 variant::toColour( ) const
    {
    math::vectorX ret( toVectorX() );
    if( ret.size() >= 3 )
        {
        return math::colour4( ret );
        }
    else if( ret.size() == 2 )
        {
        return math::colour4( ret[0], ret[1], 0.0 );
        }
    else if( ret.size() == 1 )
        {
        return math::colour4( ret[0] );
        }
    return math::colour4::WHITE;
    }

void variant::fromQuaternion( const math::quaternion &in )
    {
    fromVectorX( in );
    }

math::quaternion variant::toQuaternion( ) const
    {
    math::vectorX ret( toVectorX() );
    if( ret.size() == 4 )
        {
        return math::quaternion( ret ).normalise();
        }
    return math::quaternion::IDENTITY;
    }

void variant::fromVectorX( const math::vectorX &in )
    {
    clear();
    _other = new math::vectorX( in );
    _type = VectorX;
    }

math::vectorX variant::toVectorX( ) const
    {
    if( _type == String || _type == QtVariant )
        {
        std::istringstream stream;
        if( _type == String )
            {
            stream.str( *reinterpret_cast <string *> (_other) );
            }
        else if( _type == QtVariant )
            {
            stream.str( reinterpret_cast <QVariant *> (_other)->toString().toStdString() );
            }

        vector <JFLOAT> ret;
        std::streampos pos = -1;
        while( !stream.eof() && pos != stream.tellg( ) )
            {
            pos = stream.tellg();
            JFLOAT temp;
            stream>>temp;
            ret.push_back( temp );
            }

        return math::vectorX( ret.size(), &(ret.front()) );
        }
    else if( _type ==  Bool )
        {
        return math::vectorX( 1, _bool );
        }
    else if( _type == Int )
        {
        return math::vectorX( 1, _int );
        }
    else if( _type == UnsignedInt )
        {
        return math::vectorX( 1, _uint );;
        }
    else if( _type == Char )
        {
        return math::vectorX( 1, _char );
        }
    else if( _type == Double )
        {
        return math::vectorX( 1, _double );
        }
    else if( _type == VectorX )
        {
        return *reinterpret_cast <math::vectorX *> (_other);
        }
    else if( _type == List  && reinterpret_cast <list <variant> *> (_other)->size() )
        {
        return reinterpret_cast <list <variant> *> (_other)->front().toVectorX();
        }
    return math::vectorX( 1, 0.0 );
    }


void variant::fromList( const list<variant> &in )
    {
    clear();
    _other = new list<variant>( in );
    _type = List;
    }

list <variant> variant::toList( ) const
    {
    if( _type == List )
        {
        return *reinterpret_cast <list <variant> *> (_other);
        }
    else
        {
        list<variant> ret;
        variant el( *this );
        ret.push_back( el );
        return ret;
        }
    }


void variant::fromQtVariant( const QVariant &in )
    {
    clear();
    _other = new QVariant( in );
    _type = QtVariant;
    }

QVariant variant::toQtVariant( ) const
    {
    return QVariant( QString::fromStdString( toString() ) );
    }

void variant::fromQtString( const QString &in )
    {
    fromString( in.toStdString() );
    }

QString variant::toQtString( ) const
    {
    return QString::fromStdString( toString() );
    }

END_JLIB_NAMESPACE
