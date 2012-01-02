import QtQuick 1.1

Item {
    id: nodeItem
    property alias title: header.text
    property alias colour: header.color

    function move(dx, dy)
      {
      nodeItem.x += dx;
      nodeItem.y += dy;

      nodecanvas.setNodePosition(index, Qt.vector3d(nodeItem.x, nodeItem.y, 0));
      }

    function getChildItem(index)
      {
      return propertyList.getChildItem(index);
      }

    function getInputPosition(relative)
      {
      var mapped = nodeItem.mapFromItem(relative, 0, 0);
      return mapped;
      }

    function getOutputPosition(relative)
      {
      var mapped = nodeItem.mapToItem(relative, 0, 0);
      mapped.x += node.width;
      return mapped;
      }


    state: "NotHovered"

    states: [
      State {
          name: "NotHovered"
          PropertyChanges {
            target: fader
            color: "#282828"
          }
      },
      State {
          name: "Hovered"
          PropertyChanges {
            target: fader
            color: "#3F3F3F"
          }
      }
    ]

    transitions: [
      Transition {
        from: "*"; to: "*"
        ColorAnimation { properties: "color"; easing.type: Easing.OutBounce; duration: 1000 }
      }
    ]

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

        gradient: Gradient {
          GradientStop {
            position: 0.0
            color: "#282828"
          }
          GradientStop {
            id: fader
            position: 1.0
            color: "#3F3F3F"
          }
        }

        border.color: "#666666"
        radius: 4

        MouseArea {
            anchors.fill: contents
            hoverEnabled: true

            onEntered: nodeItem.state = "Hovered"
            onExited: nodeItem.state = "NotHovered"
            onDoubleClicked: nodecanvas.setRootToChildIndex(index)
            onClicked: nodecanvas.bringToTop(nodeItem);
        }

        Column {
            id: contents
            x: 3
            y: 3
            width: 114
            height: childrenRect.height
            spacing: 2

            NodeHeader {
              id: header
              width: contents.width

              onDragged: {
                move(x, y);
              }
            }

            PropertyList {
              id: propertyList
              rootIndex: nodecanvas.childIndex(index)
            }
        }
    }
}
