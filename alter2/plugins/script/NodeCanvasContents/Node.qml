import QtQuick 1.1

Item {
    id: nodeItem
    property alias title: headerText.text
    property alias colour: header.color

    function move(dx, dy)
      {
      nodeItem.x += dx;
      nodeItem.y += dy;

      nodecanvas.setNodePosition(index, Qt.vector3d(nodeItem.x, nodeItem.y, 0));
      }

    function getChildItem(index)
      {
      return properties.itemAt(index);
      }

    Rectangle {
        property real pad: 4
        id: nodePad

        z: -0.05
        x: -pad
        y: -pad
        width: node.width + pad * 2;
        height: node.height + pad * 2;
        color: "#999999"
        border.color: "white"
        opacity: 0.3
        radius: 4 + pad
    }

    Rectangle {
        id: node
        width: 120
        height: childrenRect.height + 6
        color: "#333333"
        border.color: "#666666"
        radius: 4

        MouseArea {
            anchors.fill: contents
            hoverEnabled: true

            onEntered: parent.color = "#3F3F3F"
            onExited: parent.color = "#333333"
            onDoubleClicked: nodecanvas.setRootToChildIndex(index)
            onClicked: nodecanvas.bringToTop(nodeItem);
        }

        Column {
            id: contents
            x: 3
            y: 3
            width: 114
            height: childrenRect.height

            Rectangle {
                id: header
                width: 114
                height: 20
                border.color: Qt.darker(color, 1.4)
                border.width: 2
                radius: 3
                Text {
                    id: headerText
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    font.bold: true
                    font.pointSize: 10
                    elide: Text.ElideRight
                    color: Qt.darker(header.color, 2.0)
                }
                MouseArea {
                    property bool dragging: false
                    property real lastX: 0
                    property real lastY: 0
                    anchors.fill: parent
                    preventStealing: true
                    onPressed: {
                        nodecanvas.bringToTop(nodeItem);
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

            Rectangle {
              width: 2
              height: 2
              color: "transparent"
            }

            PropertyList {
              rootIndex: nodecanvas.childIndex(index)
            }
        }
    }
}
