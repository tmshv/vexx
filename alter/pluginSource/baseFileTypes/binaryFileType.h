#ifndef BINARYFILETYPE_H
#define BINARYFILETYPE_H

#include "APlugin"
#include "AAbstractFileType"

class binaryFileType : public AAbstractFileType
    {
public:
    binaryFileType();
    virtual QByteArray createFileContents( ) const;
    virtual void parseFileContents( const QByteArray & );
    };

#endif // BINARYFILETYPE_H
