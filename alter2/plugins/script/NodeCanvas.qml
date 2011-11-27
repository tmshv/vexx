import QtQuick 1.0
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"

  function setRootToChildIndex(childIndex)
    {
    var index = visualModel.modelIndex(childIndex);
    visualModel.rootIndex = index;
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

