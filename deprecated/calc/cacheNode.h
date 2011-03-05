#ifndef CACHENODE_H
#define CACHENODE_H

#include "nodeEntity.h"
#include "nodeRenderer.h"

class cacheNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( cacheNode, "psynapse.placid-acid.com", "Source/Cache", QColor( 204, 204, 204 ) );
public:
    cacheNode();
    ~cacheNode();

    void addVariables( jLib::graphics::shader::variableSet &set );

    virtual void relocated();
    virtual void initiate();
    virtual QRectF rect();
    virtual void setTime( int );
    virtual void prepareToRender();

private slots:
    void doUpdate();
    void aspectChanged();
    void reconnect(APropertyDevice*,AProperty*);

private:
    void rebuild();
    int _unit;
    int _frame;
    nodeRenderer _renderer;
    jLib::graphics::shader::sSampler2D _sampler;
    };

#endif // CACHENODE_H
