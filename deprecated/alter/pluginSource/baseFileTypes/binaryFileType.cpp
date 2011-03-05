#include "binaryFileType.h"

QDataStream &operator>>( QDataStream &in, binaryFileType::property &prop )
    {
    in >> prop.value;
    in >> prop.attributes;
    return in;
    }
QDataStream &operator<<( QDataStream &out, const binaryFileType::property &prop )
    {
    out << prop.value;
    out << prop.attributes;
    return out;
    }

QDataStream &operator>>( QDataStream &in, binaryFileType::entity &ent )
    {
    in >> ent.type;
    in >> ent.properties;
    return in;
    }

QDataStream &operator<<( QDataStream &out, const binaryFileType::entity &ent )
    {
    out << ent.type;
    out << ent.properties;
    return out;
    }

binaryFileType::binaryFileType() : AAbstractFileType( "Binary File" )
    {
    extensions() << "jbf";
    }

QByteArray binaryFileType::createFileContents( ) const
    {
    QByteArray ret;
    QDataStream str( &ret, QIODevice::ReadWrite );

    str << entities();

    return ret;
    }

void binaryFileType::parseFileContents( const QByteArray &arrTmp )
    {
    QByteArray arr( arrTmp );
    QDataStream str( &arr, QIODevice::ReadOnly );

    str >> entities();
    }
