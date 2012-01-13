import QtQuick 1.1
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  color: "#343434"

  property real maxNodeZ: 1.0;

  function childIndex(index)
    {
    return thisModel.modelIndex(index);
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

  function findPropertyItem(modelIndex)
    {
    if(!db.isValid(modelIndex))
      {
      return null;
      }

    if(db.isEqual(modelIndex, childModel.rootIndex))
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
    for(var i = 0, s = nodes.count; i < s; ++i)
      {
      var item = nodes.itemAt(i);
      if(item.intersect)
        {
        var hit = item.intersect(x, y);
        if(hit)
          {
          return hit;
          }
        }
      }
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
    delegate: Node
      {
      title: name
      x: propertyPosition.x
      y: propertyPosition.y
      colour: propertyColour
      }

    onItemsInserted: print("AAA")
    }

  /*VisualDataModel
    {
    id: childModel
    model: db
    rootIndex: {
      print("###");
      print(db.rowCount(db.index(0,0)));
      db.index(0, 0);
    }
    delegate: Node
      {
      title: name
      x: propertyPosition.x
      y: propertyPosition.y
      colour: propertyColour
      }
    onItemsInserted: print("BBB")
    }*/

  MouseArea
    {
    anchors.fill: parent
    onDoubleClicked: nodecanvas.setRootToParent()
    }

  Item {
    Column {
      spacing: 10
      Repeater {
        id: nodes
        model: thisModel
        onItemAdded: {
          print("A");
        }
      }

      /*Repeater {
        id: childNodes
        model: childModel
        onItemAdded: {
          print("B");
        }
      }*/
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
