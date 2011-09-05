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
      properties: ListModel
        {
        ListElement
          {
          name: "Bill Smith"
          properties: "555 3264"
          }
        ListElement
          {
          name: "John Brown"
          number: "555 8426"
          }
        }
      }
    ListElement
      {
      name: "Bill Smith"
      properties: ListModel
        {
        ListElement
          {
          name: "Bill Smith"
          properties: "555 3264"
          }
        ListElement
          {
          name: "John Brown"
          number: "555 8426"
          }
        }
      }
    ListElement
      {
      name: "Bill Smith"
      properties: ListModel
        {
        ListElement
          {
          name: "Bill Smith"
          properties: "555 3264"
          }
        ListElement
          {
          name: "John Brown"
          number: "555 8426"
          }
        }
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

