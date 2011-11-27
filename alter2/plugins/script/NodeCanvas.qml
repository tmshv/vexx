import QtQuick 1.0
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

  VisualDataModel
    {
    id: visualModel
    model: db
    delegate: Node
      {
      title: name
      x: ((Math.random() * nodecanvas.width) - 100)
      y: ((Math.random() * nodecanvas.height) - 50)
      }
    }

  MouseArea {
    anchors.fill: parent
    onDoubleClicked: nodecanvas.setRootToParent()
  }

  ListView
    {
    width: 180; height: 200
    anchors.fill: parent
    interactive: false

    model: visualModel
    }
  }

