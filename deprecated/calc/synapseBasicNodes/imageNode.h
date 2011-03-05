#ifndef IMAGENODE_H
#define IMAGENODE_H

#include "nodeEntity.h"

class imageNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( imageNode, "psynapse.placid-acid.com", "Source/Image", QColor( 204, 204, 204 ) );
public:
    imageNode();
    ~imageNode();

    virtual void addVariables( jLib::graphics::shader::variableSet &set );
    virtual void relocated();
    virtual void initiate();
    virtual QRectF rect();
    virtual void setTime( int );

private slots:
    void fileChanged( APropertyDevice*, AProperty* );

private:
    void prePropertyChanged( AProperty * );
    void rebuild();
    int _unit;
    jLib::graphics::pixel::texture *_image;
    jLib::graphics::shader::sSampler2D _sampler;
    };

#endif // IMAGENODE_H
