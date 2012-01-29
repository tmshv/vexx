import QtQuick 1.1

Rectangle {
  width: childrenRect.width+2
  height: childrenRect.height+2
  color: "transparent"

  Rectangle {
    x: 1
    y: 1
    width: 100
    height: 35 + list.height
    color: "#222222"
    border.width: 1
    border.color: "#555555"
    radius: 3

    function focusLost()
      {
      external.destroyWindow();
      }

    Timer {
      id: grabber
      interval: 1
      onTriggered: {
        external.focus();
      }
    }

    Text {
      text: "Create..."
      color: "white"
      font.bold: true
      anchors.top: parent.top
      anchors.left: parent.left
      anchors.margins: 6
    }

    Component.onCompleted: {
      external.focusLost.connect(focusLost);
      grabber.start();
    }

    ListView {
      id: list
      y: 25
      width: parent.width
      height: contentItem.childrenRect.height
      contentHeight: height

      model: ListModel {
        ListElement {
          name: "Bill Smith"
          description: "555 3264"
          }
        ListElement {
          name: "John Brown"
          description: "555 8426"
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          }
        ListElement {
          name: "Sam Wise"
          description: "555 0473"
          }
        }

      delegate: Rectangle {
        color: "#444444"
        border.width: 1
        border.color: "#333333"
        width: parent.width
        height: childrenRect.height

        Column {
          Text {
            text: name
            font.bold: true
            color: "white"
            }
          Text {
            text: description
            font.pixelSize: 9
            color: "white"
            }
          }
        }
      }
    }
  }
