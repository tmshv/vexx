import QtQuick 1.1
import VexxQMLExtensions 1.0

NodeItem {
  id: nodeItem
  property real propertyTabIn: 10
  property bool selected: false
  property variant propertiesSection: null

  driverPoint: header.driverPoint()
  drivenPoint: header.drivenPoint()
  driverNormal.x: 1.0
  driverNormal.y: 0.0
  drivenNormal.x: -1.0
  drivenNormal.y: 0.0

  function intersect(x, y)
    {
    var isect = header.intersect(x, y);
    if(isect)
      {
      return isect;
      }

    return propertyList.intersect(x, y);
    }

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
    border.color: "#666666"
    radius: 4

    state: "NotHovered"
    states: [
      State {
        name: "Selected"
        when: nodeItem.selected === true

        PropertyChanges {
          target: node
          color: "#C1BDB7"
          }
        },
      State {
          name: "NotHovered"
          when: dragMouseArea.hovered !== true

          PropertyChanges {
            target: node
            color: "#282828"
            }
        },
      State {
          name: "Hovered"
          when: dragMouseArea.hovered === true

          PropertyChanges {
            target: node
            color: "#3F3F3F"
          }
        }
      ]

    transitions: [
      Transition {
        from: "*"; to: "*"
        ColorAnimation { property: "color"; easing.type: Easing.OutBounce; duration: 500 }
        }
      ]

    MouseArea {
      id: dragMouseArea
      property bool hovered: false
      anchors.fill: contents
      hoverEnabled: true

      onEntered: hovered = true
      onExited: hovered = false
      onDoubleClicked: {
        mouse.accepted = true;
        nodecanvas.setRootIndex(nodeItem);
        }
      onPressed: {
        mouse.accepted = false;
        if(mouse.modifiers === 0)
          {
          mouse.accepted = true;
          nodecanvas.select(nodeItem, true);
          nodecanvas.bringToTop(nodeItem);
          }
        }
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
          showClose: specialMode === "" && nodeItem.deletable


          property bool shouldSelect: false

          onCreateConnection: nodecanvas.startCreatingConnection(item, mode, x, y)

          onClose: {
            nodeItem.remove();
          }

          onEnter: {
            nodecanvas.setRootIndex(nodeItem);
            }

          onClick: {
            shouldSelect = true;
            nodecanvas.bringToTop(nodeItem);
            }

          onRightClick: {
            if(propertiesSection && propertiesSection.visible)
              {
              propertiesSection.visible = false;
              return;
              }

            if(!propertiesSection)
              {
              var component = Qt.createComponent("Properties.qml");

              if(component.status === Component.Error)
                {
                // Error Handling
                console.log("Error loading Input component:", component.errorString());
                return;
                }

              propertiesSection = component.createObject(nodeItem);
              propertiesSection.x = (function() { return node.width; });
              }


            propertiesSection.visible = true;
            }

          onClickReleased: {
            if(shouldSelect)
              {
              nodecanvas.select(nodeItem, true);
              }
            }

          onDragged: {
            shouldSelect = false;
            nodecanvas.drag(nodeItem, x, y);
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
