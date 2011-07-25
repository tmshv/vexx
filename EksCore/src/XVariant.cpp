#include "XVariant"
#include "XDebug"
#include "QTextStream"
#include "QDataStream"
#include "QVariant"

#define xAs( type ) reinterpret_cast <type *>( _other )

XVariant::XVariant() : _type( Null ), _int( 0 )
    {
    }

XVariant::XVariant( const XVariant &in ) : _type( Null )
    {
    fromVariant( in );
    }

XVariant::XVariant( const char *in ) : _type( Null )
    {
    fromString( in );
    }

XVariant::XVariant( const QVariant &in ) : _type( Null )
    {
    fromString( in.toString() );
    }

XVariant::XVariant( XQuaternion in )
    {
    fromVector4D( in.toVector4D() );
    }

XVariant::~XVariant()
    {
    clear();
    }

bool XVariant::isNull() const
    {
    return _type == Null;
    }

void XVariant::clear()
    {
    if( _type == Null ||
            _type == String ||
            _type == Bool ||
            _type == Int ||
            _type == UnsignedInt ||
            _type == Char ||
            _type == Double ||
            _type == Vector2D ||
            _type == Vector3D ||
            _type == Vector4D ||
            _type == Matrix3x3 ||
            _type == Matrix4x4 ||
            _type == ByteArray ||
            _type == List )
        {
        if( _type == Vector2D )
            {
            delete reinterpret_cast <XVector2D *> ( _other );
            }
        if( _type == Vector3D )
            {
            delete reinterpret_cast <XVector3D *> ( _other );
            }
        if( _type == Vector4D )
            {
            delete reinterpret_cast <XVector4D *> ( _other );
            }
        else if( _type == String )
            {
            delete reinterpret_cast <XString *> ( _other );
            }
        else if( _type == List )
            {
            delete reinterpret_cast <XList <XVariant> *> ( _other );
            }
        if( _type == Matrix3x3 )
            {
            delete reinterpret_cast <XMatrix3x3 *> ( _other );
            }
        if( _type == Matrix4x4 )
            {
            delete reinterpret_cast <XMatrix4x4 *> ( _other );
            }
        else if( _type == QtVariant )
            {
            delete reinterpret_cast <QVariant *> ( _other );
            }
        }
    else
        {
        xError( "XVariant not cleaned effectively "<<_type );
        }
    _type = Null;
    _int = 0;
    }

XVariant &XVariant::operator =( const XVariant &in )
    {
    fromVariant( in );
    return *this;
    }

void XVariant::fromVariant( const XVariant &in )
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
    else if( in._type == Vector2D )
        {
        fromVector2D( in.toVector2D() );
        }
    else if( in._type == Vector3D )
        {
        fromVector3D( in.toVector3D() );
        }
    else if( in._type == Vector4D )
        {
        fromVector4D( in.toVector4D() );
        }
    else if( in._type == List )
        {
        fromList( in.toList() );
        }
    else if( in._type == Matrix3x3 )
        {
        fromMatrix3x3( in.toMatrix3x3() );
        }
    else if( in._type == Matrix4x4 )
        {
        fromMatrix4x4( in.toMatrix4x4() );
        }
    else if( in._type == ByteArray )
        {
        fromByteArray( in.toByteArray() );
        }
    else if( in._type == QtVariant )
        {
        fromQVariant( in.toQVariant() );
        }
    }


void XVariant::fromQuaternion( const XQuaternion &in )
    {
    fromVector4D( in.toVector4D() );
    }

void XVariant::fromCharString( const char *in )
    {
    fromString( in );
    }

void XVariant::fromColour( const XColour &in )
    {
    clear();
    _type = Vector4D;
    _other = new XVector4D( in );
    }


void XVariant::fromQVariant( const QVariant &in )
    {
    fromString( in.toString() );
    }

void XVariant::fromBool( bool in )
    {
    clear();
    _type = Bool;
    _bool = in;
    }

void XVariant::fromInt( int in )
    {
    clear();
    _type = Int;
    _int = in;
    }

void XVariant::fromUnsignedInt( unsigned int in )
    {
    clear();
    _type = UnsignedInt;
    _uint = in;
    }

void XVariant::fromChar( char in )
    {
    clear();
    _type = Char;
    _char = in;
    }

void XVariant::fromDouble( double in )
    {
    clear();
    _type = Double;
    _double = in;
    }

bool XVariant::toBool( ) const
    {
    switch( _type )
        {
        case Null: return false;
        case Bool: return _bool;
        case Int: return _int;
        case UnsignedInt:  return _uint;
        case Char: return _char;
        case Double: return _double;
        case String: return xAs( XString )->toInt();
        case Vector2D: return xAs( XVector2D )->x();
        case Vector3D: return xAs( XVector3D )->x();
        case Vector4D: return xAs( XVector4D )->x();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toBool();
        case Matrix3x3: return xAs( XMatrix3x3 )->data()[0];
        case Matrix4x4: return xAs( XMatrix4x4 )->data()[0];
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); bool ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->toBool();
        }
    return false;
    }

int XVariant::toInt( ) const
    {
    switch( _type )
        {
        case Null: return 0;
        case Bool: return _bool;
        case Int: return _int;
        case UnsignedInt:  return _uint;
        case Char: return _char;
        case Double: return _double;
        case String: return xAs( XString )->toInt();
        case Vector2D: return xAs( XVector2D )->x();
        case Vector3D: return xAs( XVector3D )->x();
        case Vector4D: return xAs( XVector4D )->x();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toInt();
        case Matrix3x3: return xAs( XMatrix3x3 )->data()[0];
        case Matrix4x4: return xAs( XMatrix4x4 )->data()[0];
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); int ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->toInt();
        }
    return 0;
    }

unsigned int XVariant::toUnsignedInt( ) const
    {
    switch( _type )
        {
        case Null: return 0;
        case Bool: return _bool;
        case Int: return _int;
        case UnsignedInt:  return _uint;
        case Char: return _char;
        case Double: return _double;
        case String: return xAs( XString )->toUInt();
        case Vector2D: return xAs( XVector2D )->x();
        case Vector3D: return xAs( XVector3D )->x();
        case Vector4D: return xAs( XVector4D )->x();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toUnsignedInt();
        case Matrix3x3: return xAs( XMatrix3x3 )->data()[0];
        case Matrix4x4: return xAs( XMatrix4x4 )->data()[0];
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); unsigned int ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->toUInt();
        }
    return 0;
    }

char XVariant::toChar() const
    {
    switch( _type )
        {
        case Null: return 0;
        case Bool: return _bool;
        case Int: return _int;
        case UnsignedInt:  return _uint;
        case Char: return _char;
        case Double: return _double;
        case String: return xAs( XString )->toInt();
        case Vector2D: return xAs( XVector2D )->x();
        case Vector3D: return xAs( XVector3D )->x();
        case Vector4D: return xAs( XVector4D )->x();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toChar();
        case Matrix3x3: return xAs( XMatrix3x3 )->data()[0];
        case Matrix4x4: return xAs( XMatrix4x4 )->data()[0];
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); int ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->toChar().toAscii();
        }
    return 0;
    }

double XVariant::toDouble() const
    {
    switch( _type )
        {
        case Null: return 0.0;
        case Bool: return _bool;
        case Int: return _int;
        case UnsignedInt:  return _uint;
        case Char: return _char;
        case Double: return _double;
        case String: return xAs( XString )->toDouble();
        case Vector2D: return xAs( XVector2D )->x();
        case Vector3D: return xAs( XVector3D )->x();
        case Vector4D: return xAs( XVector4D )->x();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toDouble();
        case Matrix3x3: return xAs( XMatrix3x3 )->data()[0];
        case Matrix4x4: return xAs( XMatrix4x4 )->data()[0];
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); double ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->toDouble();
        }
    return 0.0;
    }

XColour XVariant::toColour() const
    {
    return toVector4D();
    }

XQuaternion XVariant::toQuaternion() const
    {
    return XQuaternion( toVector4D() );
    }

QVariant XVariant::toQVariant() const
    {
    switch( _type )
        {
        case Null: return QVariant();
        case Bool: return QVariant( _bool );
        case Int: return QVariant( _int );
        case UnsignedInt: return QVariant( _uint );
        case Char: return QVariant( _char );
        case Double: return QVariant( _double );
        case String: return QVariant( *xAs( XString ) );
        case Vector2D: return QVariant( *xAs( XVector2D ) );
        case Vector3D: return QVariant( *xAs( XVector3D ) );
        case Vector4D: return QVariant( *xAs( XVector4D ) );
        case List:
            {
            QVariantList ret;
            foreach( const XVariant &it, *xAs( XVariantList ) )
                {
                ret << it.toQVariant();
                }
            return QVariant( ret );
            }
        case Matrix3x3: return QVariant( QMatrix4x4( *xAs( XMatrix3x3 ) ) );
        case Matrix4x4: return QVariant( *xAs( XMatrix4x4 ) );
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); QVariant ret; stream >> ret; return ret; }
        case QtVariant: return *xAs( QVariant );
        }
    return QVariant();
    }

XString XVariant::toString() const
    {
    switch( _type )
        {
        case Null: return XString();
        case Bool: return XString::number( _bool );
        case Int: return XString::number( _int );
        case UnsignedInt: return XString::number( _uint );
        case Char: return XString::number( _char );
        case Double: return XString::number( _double );
        case String: return *xAs( XString );
        case Vector2D: return XString::number( xAs( XVector2D )->x() ) + " " +
                              XString::number( xAs( XVector2D )->y() );
        case Vector3D: return XString::number( xAs( XVector3D )->x() ) + " " +
                              XString::number( xAs( XVector3D )->y() ) + " " +
                              XString::number( xAs( XVector3D )->z() );
        case Vector4D: return XString::number( xAs( XVector4D )->x() ) + " " +
                              XString::number( xAs( XVector4D )->y() ) + " " +
                              XString::number( xAs( XVector4D )->z() ) + " " +
                              XString::number( xAs( XVector4D )->w() );
        case List:
            {
            QString ret = "XList ";
            XVariantList *list( xAs( XVariantList ) );
            ret += QString::number( list->size() );

            foreach( XVariant var, *list )
                {
                QString toStr( var.toString() );
                ret += " " + QString::number( toStr.length() ) + " " + toStr;
                }

            return ret;
            }
        case Matrix3x3:
            {
            QString ret;
            for( unsigned int x=0; x<9; x++ )
                {
                ret += QString::number( xAs( XMatrix3x3 )->data()[x] );
                if( x < 8 )
                    {
                    ret += " ";
                    }
                }
            return ret;
            }
        case Matrix4x4:
            {
            QString ret;
            for( unsigned int x=0; x<16; x++ )
                {
                ret += QString::number( xAs( XMatrix4x4 )->data()[x] );
                if( x < 15 )
                    {
                    ret += " ";
                    }
                }
            return ret;
            }
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XString ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->toString();
        }
    return XString();
    }

XVector2D XVariant::toVector2D() const
    {
    switch( _type )
        {
        case Null: return XVector2D();
        case Bool: return XVector2D( _bool, 0.0 );
        case Int: return XVector2D( _int, 0.0 );
        case UnsignedInt: return XVector2D( _uint, 0.0 );
        case Char: return XVector2D( _char, 0.0 );
        case Double: return XVector2D( _double, 0.0 );
        case String:
            {
            QTextStream str( xAs( XString ) );
            xReal x, y;
            str >> x >> y;
            return XVector2D( x, y );
            }
        case Vector2D: return *xAs( XVector2D );
        case Vector3D: return xAs( XVector3D )->toVector2D();
        case Vector4D: return xAs( XVector4D )->toVector2D();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toVector2D();
        case Matrix3x3: return XVector2D( xAs( XMatrix3x3 )->data()[0], xAs( XMatrix3x3 )->data()[1] );
        case Matrix4x4: return XVector2D( xAs( XMatrix4x4 )->data()[0], xAs( XMatrix4x4 )->data()[1] );
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XVector2D ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->value<XVector2D>();
        }
    return XVector2D();
    }

XVector3D XVariant::toVector3D() const
    {
    switch( _type )
        {
        case Null: return XVector3D();
        case Bool: return XVector3D( _bool, 0.0, 0.0 );
        case Int: return XVector3D( _int, 0.0, 0.0 );
        case UnsignedInt: return XVector3D( _uint, 0.0, 0.0 );
        case Char: return XVector3D( _char, 0.0, 0.0 );
        case Double: return XVector3D( _double, 0.0, 0.0 );
        case String:
            {
            QTextStream str( xAs( XString ) );
            xReal x, y, z;
            str >> x >> y >> z;
            return XVector3D( x, y, z );
            }
        case Vector2D: return xAs( XVector2D )->toVector3D();
        case Vector3D: return *xAs( XVector3D );
        case Vector4D: return xAs( XVector4D )->toVector3D();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toVector3D();
        case Matrix3x3: return XVector3D( xAs( XMatrix3x3 )->data()[0], xAs( XMatrix3x3 )->data()[1], xAs( XMatrix3x3 )->data()[2] );
        case Matrix4x4: return XVector3D( xAs( XMatrix4x4 )->data()[0], xAs( XMatrix4x4 )->data()[1], xAs( XMatrix4x4 )->data()[2] );
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XVector3D ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->value<XVector3D>();
        }
    return XVector3D();
    }

XVector4D XVariant::toVector4D() const
    {
    switch( _type )
        {
        case Null: return XVector4D();
        case Bool: return XVector4D( _bool, 0.0, 0.0, 0.0 );
        case Int: return XVector4D( _int, 0.0, 0.0, 0.0 );
        case UnsignedInt: return XVector4D( _uint, 0.0, 0.0, 0.0 );
        case Char: return XVector4D( _char, 0.0, 0.0, 0.0 );
        case Double: return XVector4D( _double, 0.0, 0.0, 0.0 );
        case String:
            {
            QTextStream str( xAs( XString ) );
            xReal x, y, z, w;
            str >> x >> y >> z >> w;
            return XVector4D( x, y, z, w );
            }
        case Vector2D: return xAs( XVector2D )->toVector4D();
        case Vector3D: return xAs( XVector3D )->toVector4D();
        case Vector4D: return *xAs( XVector4D );
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toVector4D();
        case Matrix3x3: return XVector4D( xAs( XMatrix3x3 )->data()[0], xAs( XMatrix3x3 )->data()[1], xAs( XMatrix3x3 )->data()[2], xAs( XMatrix3x3 )->data()[3] );
        case Matrix4x4: return XVector4D( xAs( XMatrix4x4 )->data()[0], xAs( XMatrix4x4 )->data()[1], xAs( XMatrix4x4 )->data()[2], xAs( XMatrix4x4 )->data()[3] );
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XVector4D ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->value<XVector4D>();
        }
    return XVector4D();
    }

XMatrix3x3 XVariant::toMatrix3x3() const
    {
    switch( _type )
        {
        case Null: return XMatrix3x3();
        case Bool: return XMatrix3x3();
        case Int: return XMatrix3x3();
        case UnsignedInt: return XMatrix3x3();
        case Char: return XMatrix3x3();
        case Double: return XMatrix3x3();
        case String:
            {
            QTextStream str( xAs( XString ) );
            XMatrix3x3 ret;
            for( unsigned int x=0; x<9; x++ )
                {
                xReal w = 0.0;
                str >> w;
                ret.data()[x] = w;
                }
            return ret;
            }
        case Vector2D: return XMatrix3x3();
        case Vector3D: return XMatrix3x3();
        case Vector4D: return XMatrix3x3();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toMatrix3x3();
        case Matrix3x3: return *xAs( XMatrix3x3 );
        case Matrix4x4: return xAs( XMatrix4x4 )->normalMatrix();
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XMatrix3x3 ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->value<XMatrix3x3>();
        }
    return XMatrix3x3();
    }

XMatrix4x4 XVariant::toMatrix4x4() const
    {
    switch( _type )
        {
        case Null: return XMatrix4x4();
        case Bool: return XMatrix4x4();
        case Int: return XMatrix4x4();
        case UnsignedInt: return XMatrix4x4();
        case Char: return XMatrix4x4();
        case Double: return XMatrix4x4();
        case String:
            {
            QTextStream str( xAs( XString ) );
            XMatrix4x4 ret;
            for( unsigned int x=0; x<16; x++ )
                {
                xReal w = 0.0;
                str >> w;
                ret.data()[x] = w;
                }
            return ret;
            }
        case Vector2D: return XMatrix4x4();
        case Vector3D: return XMatrix4x4();
        case Vector4D: return XMatrix4x4();
        case List: if( !xAs( XVariantList )->isEmpty() ) return xAs( XVariantList )->front().toMatrix4x4();
        case Matrix3x3: return XMatrix4x4( *xAs( XMatrix3x3 ) );
        case Matrix4x4: return *xAs( XMatrix4x4 );
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XMatrix4x4 ret; stream >> ret; return ret; }
        case QtVariant: return xAs( QVariant )->value<XMatrix4x4>();
        }
    return XMatrix4x4();
    }

QByteArray XVariant::toByteArray() const
    {
    switch( _type )
        {
        case Null: return QByteArray();
        case Bool: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << _bool; return arr; }
        case Int: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << _int; return arr; }
        case UnsignedInt: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << _uint; return arr; }
        case Char: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << _char; return arr; }
        case Double: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << _double; return arr; }
        case String: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XString ); return arr; }
        case Vector2D: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XVector2D ); return arr; }
        case Vector3D: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XVector3D ); return arr; }
        case Vector4D: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XVector4D ); return arr; }
        case List: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XVariantList ); return arr; }
        case Matrix3x3: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XMatrix3x3 ); return arr; }
        case Matrix4x4: { QByteArray arr; QDataStream stream( &arr, QIODevice::WriteOnly ); stream << *xAs( XMatrix4x4 ); return arr; }
        case ByteArray: return *xAs( QByteArray );
        case QtVariant: return xAs( QVariant )->toByteArray();
        }
    return QByteArray();
    }

XVariantList XVariant::toList() const
    {
    switch( _type )
        {
        case Null: return XVariantList();
        case Bool: return XVariantList() << _bool;
        case Int: return XVariantList() << _int;
        case UnsignedInt: return XVariantList() << _uint;
        case Char: return XVariantList() << _char;
        case Double: return XVariantList() << _double;
        case String:
            {
            XString str( *xAs( XString ) );
            if( str.startsWith( "XList ") )
                {
                QString list( str.mid( 6 ) );
                QTextStream stream( &list );
                unsigned int num;
                stream >> num;
                XVariantList ret;
                for( unsigned int x=0; x<num; x++ )
                    {
                    unsigned int len;
                    stream >> len;
                    stream.seek( stream.pos() + 1 );
                    ret << stream.read( len );
                    }
                return ret;
                }
            return XVariantList() << str;
            }
        case Vector2D: return XVariantList() << *xAs( XVector2D );
        case Vector3D: return XVariantList() << *xAs( XVector2D );
        case Vector4D: return XVariantList() << *xAs( XVector2D );
        case List: return *xAs( XVariantList );
        case Matrix3x3: return XVariantList() << *xAs( XMatrix3x3 );
        case Matrix4x4: return XVariantList() << *xAs( XMatrix4x4 );
        case ByteArray: { QDataStream stream( xAs( QByteArray ), QIODevice::ReadOnly ); XVariantList ret; stream >> ret; return ret; }
        case QtVariant:
            {
            XVariantList ret;
            QVariantList temp( xAs( QVariant )->toList() );

            foreach( const QVariant var, temp )
                {
                ret << var;
                }

            return ret;
            }
        }
    return XVariantList();
    }

QDebug operator<<( QDebug str, const XVariant &in )
    {
    return str << in.toString();
    }

QDataStream &operator<<( QDataStream &stream, const XVariant &var )
    {
    stream << var.toPortable( XVariant::Binary );
    return stream;
    }

QDataStream &operator>>( QDataStream &stream, XVariant &var )
    {
    QByteArray arr;
    stream >> arr;
    var.fromPortable( arr, XVariant::Binary );
    return stream;
    }

QByteArray XVariant::toPortable( PortableType type ) const
    {
    if( type == Ascii )
        {
        return toString().toUtf8();
        }
    return toByteArray();
    }

void XVariant::fromPortable( QByteArray in, PortableType type )
    {
    if( type == Ascii )
        {
        fromString( QString::fromUtf8( in ) );
        }
    else
        {
        fromByteArray( in );
        }
    }
