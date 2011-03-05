#ifndef XMLFILETYPE_H
#define XMLFILETYPE_H

#include "APlugin"
#include "AAbstractFileType"

class xmlFileType : public AAbstractFileType
    {
public:
    xmlFileType();
    virtual QByteArray createFileContents( ) const;
    virtual void parseFileContents( const QByteArray & );
    };

#endif
