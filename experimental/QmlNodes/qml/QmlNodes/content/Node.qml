import QtQuick 1.0

Item {
  function move(dx, dy) {
    node.x += dx
    node.y += dy
  }
  Rectangle {
    id: node
    x: 100
    y: 100
    width: 120
    height: 80
    color: "red"
    border.color: "blue"
    radius: 4
    Column {
      anchors.margins: 3
      anchors.fill: parent

      Rectangle {
        width: parent.width
        height: 20
        color: "green"
        border.color: "blue"
        radius: 3
        Text {
          anchors.centerIn: parent
          text: "Hello World"
        }
        MouseArea {
          property bool dragging: false
          property real lastX: 0
          property real lastY: 0
          anchors.fill: parent
          onPressed: {
            dragging = true
            lastX = mouse.x
            lastY = mouse.y
          }
          onReleased: dragging = false
          onMousePositionChanged: {
            var gc = mapFromItem(parent.parent.parent.parent.parent, mouse.x, mouse.y)
            print("#" + lastX + " " + lastY)
            parent.parent.parent.parent.move(gc.x - lastX, gc.y - lastY)
            lastX = gc.x
            lastY = gc.y
            print(lastX + " " + lastY)
          }
        }
      }
    }
  }
}
