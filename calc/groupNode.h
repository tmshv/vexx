#ifndef GROUPNODE_H
#define GROUPNODE_H

#include "nodeEntity.h"

class groupNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( groupNode, "psynapse.placid-acid.com", "Developer/Group", QColor( 204, 204, 204 ) );
public:
    groupNode();

    void relocated();

protected:
    virtual void onChildrenChange();

private slots:
    void updateInternalProperties(APropertyDevice*,AProperty*);

private:
    bool _loading;
    };

#endif // GROUPNODE_H
