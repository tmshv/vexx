#ifndef VIDEONODE_H
#define VIDEONODE_H

#include "nodeEntity.h"

class videoNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( videoNode, "psynapse.placid-acid.com", "Source/Video", QColor( 204, 204, 204 ) );
public:
    videoNode();
    ~videoNode();

    virtual void addVariables( jLib::graphics::shader::variableSet &set );
    virtual void relocated();
    virtual QRectF rect();
    virtual void initiate();
    virtual void setTime( int );

private slots:
    void setupFile( APropertyDevice*, AProperty* );

private:
    void prePropertyChanged( AProperty * );
    void rebuild();
    int _unit;
    jLib::graphics::pixel::texture *_image;
    jLib::graphics::shader::sSampler2D _sampler;
    };

#endif // VIDEONODE_H
