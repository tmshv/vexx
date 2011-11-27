import QtQuick 1.0

Item {
  property string title: "Node"

  function move(dx, dy)
    {
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

    MouseArea {
      anchors.fill: parent
      hoverEnabled: true

      onEntered: parent.color = "blue"
      onExited: parent.color = "red"
      onDoubleClicked: nodecanvas.setRootToChildIndex(index)
    }

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
          text: parent.parent.parent.parent.title
        }
        MouseArea {
          property bool dragging: false
          property real lastX: 0
          property real lastY: 0
          anchors.fill: parent
          onPressed: {
            dragging = true
            var gc = mapToItem(parent.parent.parent.parent.parent, mouse.x, mouse.y)
            lastX = gc.x
            lastY = gc.y
          }
          onDoubleClicked: nodecanvas.setRootToChildIndex(index)
          onReleased: dragging = false
          onMousePositionChanged: {
            var gc = mapToItem(parent.parent.parent.parent.parent, mouse.x, mouse.y)
            parent.parent.parent.parent.move(gc.x - lastX, gc.y - lastY)
            lastX = gc.x
            lastY = gc.y
          }
        }
      }
    }
  }
}
