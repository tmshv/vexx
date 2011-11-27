import QtQuick 1.0
//import "NodeCanvasContents"

Item {
    property string title: "Node"

    function move(dx, dy) {
        node.x += dx
        node.y += dy
    }

    Rectangle {
        id: node
        width: 120
        height: childrenRect.height + 6
        color: "red"
        border.color: "blue"
        radius: 4

        MouseArea {
            anchors.fill: contents
            hoverEnabled: true

            onEntered: parent.color = "blue"
            onExited: parent.color = "red"
            onDoubleClicked: nodecanvas.setRootToChildIndex(index)
        }

        Column {
            id: contents
            x: 3
            y: 3
            width: 114
            height: childrenRect.height

            Rectangle {
                width: 114
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

            Rectangle {
                width: 114
                height: childrenRect.height
                color: "yellow"

                VisualDataModel {
                    id: chilrenVisualModel
                    model: db
                    delegate: Property {
                        text: name
                    }
                    rootIndex: nodecanvas.childIndex(index)
                }

                ListView {
                    width: parent.width
                    height: contentItem.childrenRect.height + 100
                    interactive: false
                    model: chilrenVisualModel
                }
            }
        }
    }
}
