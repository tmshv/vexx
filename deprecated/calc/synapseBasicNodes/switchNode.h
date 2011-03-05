#ifndef SWITCHNODE_H
#define SWITCHNODE_H

#include "nodeEntity.h"

class switchNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( switchNode, "psynapse.placid-acid.com", "Channels/Switch", QColor( 207, 195, 183 ) );
public:
    switchNode();

private slots:
    void updateSource();
    };

#endif // REORDERNODE_H
