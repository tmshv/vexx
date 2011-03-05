#ifndef IIMAGE_H
#define IIMAGE_H

#include "QDomElement"
#include "QAtomicInt"
#include "QImage"

class IImage
    {
public:
    IImage( QString directory, QDomElement myTag, QDomDocument *doc );
    ~IImage();

    QString filename() const;

    bool isPreparingThumbnail() const;
    void prepareThumbnail( int size );
    bool thumbnailReady() const;
    QImage thumbnail() const;

private:
    QDomElement _tag;
    QString _dir;
    void parse();
    QAtomicInt _preparing;
    QImage _thumbnail;
    bool _valid;
    QDomDocument *_doc;
    };

#endif // IIMAGE_H
