import QtQuick 1.1
import VexxQMLExtensions 1.0

Rectangle {
  id: rect
  property alias text: label.text

  signal clicked()

  MouseArea {
    anchors.fill: parent
    hoverEnabled: true
    onHoveredChanged: rect.border.width = containsMouse ? 1.0 : 0.0
    onClicked: rect.clicked()
  }

  border.width: 0.0
  border.color: "#777777"
  color: "black"
  radius: 2
  width: label.width + 4
  height: label.height + 4

  Text {
    id: label
    anchors.centerIn: parent
    font.bold: true
    color: "white"
  }
}
