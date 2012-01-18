import QtQuick 1.1

Item {
    id: nodeItem
    property alias title: header.text
    property alias colour: header.color
    property real propertyTabIn: 10
    property alias modelIndex: propertyList.rootIndex
    property alias propertyMask: propertyList.propertyMask
    property string externalMode: ""

    function move(dx, dy)
      {
      nodeItem.x += dx;
      nodeItem.y += dy;

      if(externalMode === "")
        {
        nodecanvas.setNodePosition(index, Qt.vector3d(nodeItem.x, nodeItem.y, 0));
        }
      }

    function getModelIndex()
      {
      if(externalMode === "")
        {
        return nodecanvas.childIndex(index);
        }
      return null;
      }

    function childItem(index)
      {
      print("ChildItem from", nodeItem.title);
      return propertyList.childItem(index);
      }

    function getInputPosition(relative)
      {
      var mapped = nodeItem.mapToItem(relative, 0, header.height/2 + 2);
      return Qt.point(mapped.x, mapped.y);
      }

    function getOutputPosition(relative)
      {
      var mapped = header.mapToItem(relative, header.width, header.height/2 + 2);
      return Qt.point(mapped.x, mapped.y);
      }

    function intersect(x, y)
      {
      var isect = header.intersect(x, y);
      if(isect)
        {
        return isect;
        }

      /*for(var i = 0, s = nodes.count; i < s; ++i)
        {
        var item = nodes.itemAt(i);
        var isect = item.intersect;
        if(isect)
          {
          var hit = isect(x, y);
          if(hit)
            {
            return hit;
            }
          }
        }*/

      return null;
      }

    state: "NotHovered"

    states: [
      State {
          name: "NotHovered"
          when: dragMouseArea.hovered !== true

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
          when: dragMouseArea.hovered === true

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
          when: dragMouseArea.pressed === true

          PropertyChanges {
            target: node
            smooth: false
          }
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
            id: dragMouseArea
            property bool hovered: false
            anchors.fill: contents
            hoverEnabled: true

            onEntered: hovered = true
            onExited: hovered = false
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
                showInterfaces: externalMode === ""

                onCreateConnection: nodecanvas.startCreatingConnection(item, mode, x, y)

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
                showInternalProperties: externalMode !== ""
              }
            }
        }
    }
}
