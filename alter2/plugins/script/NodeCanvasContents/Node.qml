import QtQuick 1.1
import VexxQMLExtensions 1.0

NodeItem {
    id: nodeItem
    property real propertyTabIn: 10

    state: "NotHovered"

    driverPoint: header.driverPoint()
    drivenPoint: header.drivenPoint()
    driverNormal.x: 1.0
    driverNormal.y: 0.0
    drivenNormal.x: -1.0
    drivenNormal.y: 0.0

    states: [
      State {
          name: "NotHovered"
          when: dragMouseArea.hovered !== true

          /*PropertyChanges {
            target: fader
            color: "#282828"
          }*/
      },
      State {
          name: "Hovered"
          when: dragMouseArea.hovered === true

          /*PropertyChanges {
            target: fader
            color: "#3F3F3F"
          }*/
      }
    ]

    transitions: [
      Transition {
        from: "*"; to: "*"
        ColorAnimation { property: "color"; easing.type: Easing.OutBounce; duration: 1000 }
      }
    ]

    Rectangle {
        property real pad: 3
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
        width: 121
        height: childrenRect.height + 6
        smooth: true
        color: "#282828"
        border.color: "#666666"
        radius: 4

        /*gradient: Gradient {
          GradientStop {
            position: 0.0
            color: "#282828"
          }
          GradientStop {
            id: fader
            position: 1.0
            color: "#3F3F3F"
          }
        }*/

        MouseArea {
            id: dragMouseArea
            property bool hovered: false
            anchors.fill: contents
            hoverEnabled: true

            onEntered: hovered = true
            onExited: hovered = false
            onDoubleClicked: nodecanvas.setRootIndex(nodeItem)
            onClicked: nodecanvas.bringToTop(nodeItem);
        }

        Column {
            id: contents
            width: node.width
            height: childrenRect.height
            spacing: 4

            Item {
              width: header.width + header.x
              height: header.height + header.y
              NodeHeader {
                id: header
                x: 2
                y: 3
                color: nodeItem.colour
                text: nodeItem.name
                width: contents.width - 4
                showInterfaces: specialMode === ""

                //onCreateConnection: nodecanvas.startCreatingConnection(item, mode, x, y)

                onEnter: {
                  nodecanvas.setRootIndex(nodeItem);
                }

                onBringToTop: {
                  nodecanvas.bringToTop(nodeItem);
                }

                onDragged: {
                  nodeItem.setPosition(nodeItem.x + x, nodeItem.y + y);
                }
              }
            }

            PropertyList {
              id: propertyList
              width: node.width
              xOffset: 0
              rootItem: nodeItem
            }
        }
    }
}
