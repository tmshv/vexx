import QtQuick 1.1

Row {
  id: grouper
  property alias text: label.text
  property alias colour: colourBlob.color

  Text {
    text: " "
  }

  Rectangle {
      id: colourBlob
      y: 2
      border.color: Qt.darker(color, 1.4)
      border.width: 1
      radius: 5
      width: 10
      height: 10
  }

  Text {
    text: "  "
  }

  Text {
      id: label
  }
}
