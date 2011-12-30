import QtQuick 1.1

Item {
    id: nodeItem
    property alias title: headerText.text
    property alias colour: header.color

    function move(dx, dy) {
        nodeItem.x += dx;
        nodeItem.y += dy;

        nodecanvas.setNodePosition(index, Qt.vector3d(nodeItem.x, nodeItem.y, 0));
    }

    Rectangle {
        id: node
        width: 120
        height: childrenRect.height + 6
        color: "#767676"
        border.color: "#898989"
        radius: 4

        MouseArea {
            anchors.fill: contents
            hoverEnabled: true

            onEntered: parent.color = "#808080"
            onExited: parent.color = "#767676"
            onDoubleClicked: nodecanvas.setRootToChildIndex(index)
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
                    anchors.centerIn: parent
                    font.bold: true
                    color: Qt.darker(header.color, 2.0)
                }
                MouseArea {
                    property bool dragging: false
                    property real lastX: 0
                    property real lastY: 0
                    anchors.fill: parent
                    preventStealing: true
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

            Rectangle {
              width: 2
              height: 2
              color: "transparent"
            }

            Rectangle {
                width: 114
                height: childrenRect.height
                color: "transparent"

                VisualDataModel {
                    id: chilrenVisualModel
                    model: db
                    delegate: Property {
                        text: name
                        colour: propertyColour
                    }
                    rootIndex: nodecanvas.childIndex(index)
                }

                ListView {
                    width: parent.width
                    height: contentItem.childrenRect.height + 1
                    interactive: false
                    model: chilrenVisualModel
                }
            }
        }
    }
}
