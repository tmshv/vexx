import QtQuick 1.1

Item {
    id: nodeItem
    property alias title: header.text
    property alias colour: header.color
    property real propertyTabIn: 10

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
      var mapped = nodeItem.mapToItem(relative, 0, 0);
      return mapped;
      }

    function getOutputPosition(relative)
      {
      var mapped = header.mapToItem(relative, header.width, header.height/2);
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

          PropertyChanges {
            target: node
            smooth: true
          }
      },
      State {
          name: "Hovered"

          PropertyChanges {
            target: fader
            color: "#3F3F3F"
          }

          PropertyChanges {
            target: node
            smooth: true
          }
      },
      State {
          name: "Moving"

          PropertyChanges {
            target: node
            smooth: false
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
                width: contents.width - 4

                onDraggingChanged: {
                  if(header.dragging)
                  {
                    nodeItem.state = "Moving";
                  }
                  else
                  {
                    nodeItem.state = "Hovered";
                  }
                }

                onDragged: {
                  move(x, y);
                }
              }
            }

            Item {
              height: propertyList.height
              width: node.width
              PropertyList {
                id: propertyList
                width: node.width
                xOffset: 0
                rootIndex: nodecanvas.childIndex(index)
              }
            }
        }
    }
}
