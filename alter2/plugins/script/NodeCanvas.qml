import QtQuick 1.1
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"
  function childIndex(index)
    {
    return visualModel.modelIndex(index);
    }

  function setRootToChildIndex(index)
    {
    var modelInd = childIndex(index);
    visualModel.rootIndex = modelInd;
    }

  function setRootToParent()
    {
    var index = visualModel.parentModelIndex();
    visualModel.rootIndex = index;
    }

  function setNodePosition(nodeIndex, pos)
    {
    var modelInd = childIndex(nodeIndex);
    db.setData(modelInd, "entityPosition", pos);
    }

  VisualDataModel
    {
    id: visualModel
    model: db
    delegate: Node
      {
      title: name
      x: entityPosition.x
      y: entityPosition.y
      colour: propertyColour
      }
    }

  MouseArea
    {
    anchors.fill: parent
    onDoubleClicked: nodecanvas.setRootToParent()
    }

  Repeater
    {
    id: nodes
    model: visualModel
    }
  }
