import QtQuick 1.1
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"

  property real maxNodeZ: 1.0;

  function childIndex(index)
    {
    return childModel.modelIndex(index);
    }

  function childItem(index)
    {
    return childNodes.itemAt(index);
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
    var modelInd = childModel.modelIndex(index);
    thisModel.rootIndex = modelInd;
    }

  function setRootToParent()
    {
    resetCanvas();
    var index = thisModel.parentModelIndex();
    thisModel.rootIndex = db.parent(index);
    }

  function setNodePosition(nodeIndex, pos)
    {
    var modelInd = childIndex(nodeIndex);
    db.setData(modelInd, "propertyPosition", pos);
    }

  function findPropertyItem(modelIndex, io)
    {
    if(!db.isValid(modelIndex))
      {
      print("Couldnt find root");
      return null;
      }

    if(db.isEqual(modelIndex, thisModel.rootIndex))
      {
      if(io === "input")
        {
        return [ outputs ];
        }
      else if(io === "output")
        {
        return [ inputs ];
        }
      print("incorrect io type specified", io);
      return null;
      }

    var parentIndex = db.parent(modelIndex);
    var rowIndex = db.rowIndex(modelIndex);
    if(db.isEqual(parentIndex, childModel.rootIndex))
      {
      return [ nodecanvas.childItem(rowIndex) ];
      }

    var parentItems = findPropertyItem(parentIndex, io);
    if(!parentItems)
      {
      // probably not displayed.
      print("Couldnt find parent item", db.data(parentIndex, "name"));
      return null;
      }

    var nextItem = parentItems[parentItems.length - 1].childItem(rowIndex);

    if(!nextItem)
      {
      print("Couldnt find ", db.data(parentIndex, "name"));
      return null;
      }

    parentItems.push(nextItem);
    return parentItems;
    }

  function intersectItem(item, inX, inY)
    {
    var minPos = nodecanvas.mapFromItem(item, 0, 0);
    var maxPos = nodecanvas.mapFromItem(item, item.width, item.height);
    if(inX >= minPos.x && inY >= minPos.y && inX < maxPos.x && inY < maxPos.y)
      {
      return true;
      }

    return false;
    }

  function intersect(x, y)
    {
    for(var i = 0, s = childNodes.count; i < s; ++i)
      {
      var item = childNodes.itemAt(i);
      if(item.intersect)
        {
        var hit = item.intersect(x, y);
        if(hit)
          {
          return hit;
          }
        }
      }

    // todo: input / output

    return null;
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
  function startCreatingConnection(thing, mode, x, y)
    {
    if(currentInputDragging != null)
      {
      print("Creating connection during creating connection");
      }

    currentInputDraggingItem = thing;
    currentInputBeginMode = mode;

    currentInputDraggingItem.moveDrag.connect(moveCreatingConnection);
    currentInputDraggingItem.endDrag.connect(endCreatingConnection);

    var component = Qt.createComponent("NodeCanvasContents/DynamicInput.qml");

    if(component.status === Component.Error)
      {
      // Error Handling
      console.log("Error loading Input component:", component.errorString());
      return;
      }

    currentInputDragging = component.createObject(inputGrouper, { firstColour: currentInputDraggingItem.color, lastColour: currentInputDraggingItem.color } );
    currentInputDragging.firstPoint.x = x;
    currentInputDragging.firstPoint.y = y;

    currentInputDragging.lastPoint.x = x;
    currentInputDragging.lastPoint.y = y;

    if(mode === "output")
      {
      currentInputDragging.firstNormal.x = 100
      currentInputDragging.lastNormal.x = -100
      }
    else
      {
      currentInputDragging.firstNormal.x = -100
      currentInputDragging.lastNormal.x = 100
      }
    }

  function moveCreatingConnection(x, y)
    {
    currentInputDragging.lastPoint.x = x;
    currentInputDragging.lastPoint.y = y;
    }

  function endCreatingConnection(x, y)
    {
    currentInputDragging.lastPoint.x = x;
    currentInputDragging.lastPoint.y = y;

    currentInputDraggingItem.moveDrag.disconnect(moveCreatingConnection);
    currentInputDraggingItem.endDrag.disconnect(endCreatingConnection);

    var index = null;
    var parent = currentInputDraggingItem;
    while(parent && index == null)
      {
      if(parent.getModelIndex)
        {
        index = parent.getModelIndex();
        break;
        }

      parent = parent.parent;
      }

    var otherIndex = null;
    var intersectedItemParent = intersect(x, y);
    if(intersectedItemParent)
      {
      while(intersectedItemParent && otherIndex == null)
        {
        if(intersectedItemParent.getModelIndex)
          {
          otherIndex = intersectedItemParent.getModelIndex();
          break;
          }

        intersectedItemParent = intersectedItemParent.parent;
        }
      }
    else
      {
      print("no prop hit");
      }

    currentInputDragging.destroy();
    currentInputDragging = null;
    currentInputDraggingItem = null;

    if(index && otherIndex)
      {
      if(currentInputBeginMode == "input")
        {
        db.setData(index, "propertyInput", otherIndex);
        }
      else
        {
        db.setData(otherIndex, "propertyInput", index);
        }
      }
    }

  VisualDataModel
    {
    id: thisModel
    model: db
    /*delegate: Node
      {
      title: name + " " + index.toString()
      x: propertyPosition.x
      y: propertyPosition.y
      colour: propertyColour
      visible: false
      }*/
    }

  VisualDataModel
    {
    id: childModel
    model: db
    rootIndex: {
      return db.index(0, 0,thisModel.rootIndex);
    }
    delegate: Node
      {
      title: name
      x: propertyPosition.x
      y: propertyPosition.y
      colour: propertyColour
      modelIndex: childModel.modelIndex(index)
      }
    }

  MouseArea
    {
    anchors.fill: parent
    onDoubleClicked: nodecanvas.setRootToParent()
    }

  Item {
    Node {
      id: inputs
      title: "Inputs"
      x: 0
      y: 0
      colour: db.data(thisModel.rootIndex, "propertyColour")
      modelIndex: thisModel.rootIndex
      externalMode: "output"
      propertyMask: "output"
    }

    Node {
      id: outputs
      title: "Outputs"
      x: 0
      y: 0
      colour: inputs.colour
      modelIndex: thisModel.rootIndex
      externalMode: "input"
      propertyMask: "input"
    }

    /*Repeater {
      id: nodes
      model: thisModel
    }*/

    Repeater {
      id: childNodes
      model: childModel
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
