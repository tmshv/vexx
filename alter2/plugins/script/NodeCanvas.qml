import QtQuick 1.0
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"

  VisualDataModel
    {
    id: visualModel
    model: db
    delegate: Node
      {
      title: name
      }
    }

  ListView
    {
    width: 180; height: 200
    anchors.fill: parent
    interactive: false

    model: visualModel
    }
  }

