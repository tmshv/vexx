#ifndef REORDERNODE_H
#define REORDERNODE_H

#include "nodeEntity.h"

class reorderNode : public nodeEntity
    {
    Q_OBJECT
    NODE_TYPE( reorderNode, "psynapse.placid-acid.com", "Channels/Reorder", QColor( 195, 187, 164 ) );
public:
    reorderNode();

private slots:
    void updateSource();
    };

#endif // REORDERNODE_H
