// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1


Rectangle {
    id: header

    property alias text: headerText.text

    signal dragged(real x, real y)

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
            var gc = mapToItem(nodecanvas, mouse.x, mouse.y)
            lastX = gc.x
            lastY = gc.y
        }

        onDoubleClicked: nodecanvas.setRootToChildIndex(index)
        onReleased: dragging = false
        onMousePositionChanged: {
            var gc = mapToItem(nodecanvas, mouse.x, mouse.y)

            header.dragged(gc.x - lastX, gc.y - lastY)

            lastX = gc.x
            lastY = gc.y
        }
    }
}
