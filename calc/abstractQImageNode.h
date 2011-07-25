#ifndef ABSTRACTQIMAGENODE_H
#define ABSTRACTQIMAGENODE_H

#include "nodeEntity.h"
#include "synapseSDK.h"

class SYNPASE_SDK_EXPORT abstractQImageNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( abstractQImageNode, "psynapse.placid-acid.com", "Source/Painter", QColor( 204, 204, 204 ) );
public:
    ~abstractQImageNode();

protected:
    abstractQImageNode();
    void setImage( QPointF, QImage );

private slots:
    void rebuild();

private:
    virtual void addVariables( jLib::graphics::shader::variableSet &set );
    virtual QRectF rect();
    QRectF _rect;
    int _unit;
    jLib::graphics::pixel::texture *_image;
    jLib::graphics::shader::sSampler2D _sampler;
    };

#endif // PAINTERNODE_H
