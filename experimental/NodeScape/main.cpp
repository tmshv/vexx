#include <QtGui/QApplication>
#include "sccanvas.h"
#include "scnode.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScCanvas w;

    ScNode *data = new ScNode();

    // per node information
    data->setPosition(QPointF(12,12));
    data->setName("HangWang?");

    data->variables() << ScNode::Variable("Yo yo.",0)
                     << ScNode::Variable("Wang cheeseasildgunasionsadiulvniulsadfnvnsbvfsubvfsiuvbiufbfiu",0)
                     << ScNode::Variable("Wang cheese2",0)
                     << ScNode::Variable("Yo yo.",1)
                     << ScNode::Variable("Wang cheeseaVSOIUBOUADSBFVUO",1)
                     << ScNode::Variable("Wang cheese2",0);
    w.addNode(data);

    ScNode *data2 = new ScNode();

    // per node information
    data2->setPosition(QPointF(32,32));
    data2->setName("HangWang????????????????????????????????");

    data2->variables() << ScNode::Variable("Yo yo.",0)
                     << ScNode::Variable("Yo yoiujhdfionhasiofniosfnioasnfiosnfioenfiowniofnwionefioniewn.",1);
    w.addNode(data2);

    w.addConnection(data2, 0, data, 2);
    w.addConnection(data, 5, data2, 1);

    w.show();
    return a.exec();
}
