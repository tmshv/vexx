import QtQuick 1.1
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"

  property real maxNodeZ: 1.0;

  function childIndex(index)
    {
    return visualModel.modelIndex(index);
    }

  function childItem(index)
    {
    return nodes.itemAt(index);
    }

  function bringToTop(item)
    {
    maxNodeZ += 0.05;
    item.z = maxNodeZ;
    }

  function resetCanvas()
    {
    for(var i = 0, s = inputGrouper.children.length; i < s; ++i)
      {
      inputGrouper.children[i].destroy();
      }

    maxNodeZ = 1.0;
    }

  function setRootToChildIndex(index)
    {
    resetCanvas();
    var modelInd = childIndex(index);
    visualModel.rootIndex = modelInd;
    }

  function setRootToParent()
    {
    resetCanvas();
    var index = visualModel.parentModelIndex();
    visualModel.rootIndex = index;
    }

  function setNodePosition(nodeIndex, pos)
    {
    var modelInd = childIndex(nodeIndex);
    db.setData(modelInd, "propertyPosition", pos);
    }

  function findPropertyItem(modelIndex)
    {
    if(!db.isValid(modelIndex))
      {
      return null;
      }

    if(db.isEqual(modelIndex, visualModel.rootIndex))
      {
      return [ nodecanvas ];
      }

    var parentIndex = db.parent(modelIndex);
    var parentItems = findPropertyItem(parentIndex);
    if(!parentItems)
      {
      // probably not displayed.
      return null;
      }

    var rowIndex = db.rowIndex(modelIndex);

    var nextItem = parentItems[parentItems.length - 1].childItem(rowIndex);
    if(!nextItem)
      {
      return null;
      }

    parentItems.push(nextItem);

    return parentItems;
    }

  function setupInput(propertyItem, myIndex)
    {
    var inputModelIndex = db.data(myIndex, "propertyInput");
    if(db.isValid(inputModelIndex))
      {
      var component = Qt.createComponent("NodeCanvasContents/Input.qml");

      if(component.status === Component.Error)
        {
        // Error Handling
        console.log("Error loading Input component:", component.errorString());
        return;
        }

      var object = component.createObject(inputGrouper);

      object.myProperty = propertyItem;
      object.myIndex = myIndex;
      object.setupInput();

      if(propertyItem.input)
        {
        propertyItem.input.propertyChanged(propertyItem);
        }

      propertyItem.input = object;
      }
    }

  property variant currentInputDragging: null
  property variant currentInputDraggingItem: null
  property variant currentInputDraggingIndex: null
  property string currentInputBeginMode: ""
  function startCreatingConnection(index, item, mode, x, y)
    {
    if(currentInputDragging != null)
      {
      print("Creating connection during creating connection");
      }

    currentInputDraggingItem = item;
    currentInputDraggingIndex = index;
    currentInputBeginMode = "mode";

    item.moveDrag.connect(moveCreatingConnection);
    item.endDrag.connect(endCreatingConnection);

    var component = Qt.createComponent("NodeCanvasContents/DynamicInput.qml");

    if(component.status === Component.Error)
      {
      // Error Handling
      console.log("Error loading Input component:", component.errorString());
      return;
      }

    currentInputDragging = component.createObject(inputGrouper);
    }

  function moveCreatingConnection(x, y)
    {
    print("Move")
    }

  function endCreatingConnection(x, y)
    {
    currentInputDraggingItem.moveDrag.disconnect(moveCreatingConnection);
    currentInputDraggingItem.endDrag.disconnect(endCreatingConnection);

    print("End")
    currentInputDragging.destroy();
    currentInputDragging = null;
    currentInputDraggingItem = null;
    currentInputDraggingIndex = null;
    }

  VisualDataModel
    {
    id: visualModel
    model: db
    delegate: Node
      {
      title: name
      x: propertyPosition.x
      y: propertyPosition.y
      colour: propertyColour
      }
    }

  MouseArea
    {
    anchors.fill: parent
    onDoubleClicked: nodecanvas.setRootToParent()
    }

  Item {
    Repeater {
      id: nodes
      model: visualModel
    }

    Rectangle {
      id: inputGrouper
      x: 0
      y: 0
      z: -1
      width: 0
      height: 0
    }
  }
  }
