import QtQuick 1.0
import "content"

Rectangle
  {
  id: container
  width: 500; height: 400
  color: "#343434"

  ListModel
    {
    id: data

    ListElement
      {
      name: "Bill Smith"
      }
    ListElement
      {
      name: "Bill Smith"
      }
    ListElement
      {
      name: "Bill Smith"
      }
    }

  ListView
    {
    width: 180; height: 200
    anchors.fill: parent
    interactive: false

    model: data
    delegate: Node
      {
      title: name
      }
    }
  }

