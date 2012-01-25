import QtQuick 1.1
import VexxQMLExtensions 1.0
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

  function reset()
    {
    maxNodeZ = 1.0;
    }

  function bringToTop(item)
    {
    maxNodeZ += 0.05;
    item.z = maxNodeZ;
    }

  function setRootIndex(obj)
    {
    reset();
    display.setRootIndex(obj);
    }

  function setRootToParent(count)
    {
    reset();
    if(!count || count < 1)
      {
      count = 1;
      }

    display.setRootToParent(count);
    }

  function setNodePosition(nodeIndex, pos)
    {
    var modelInd = childIndex(nodeIndex);
    db.setData(modelInd, "propertyPosition", pos);
    }

  function setInputsPosition(pos)
    {
    db.setData(thisModel.rootIndex, "entityInputPosition", pos);
    }

  function setOutputsPosition(pos)
    {
    db.setData(thisModel.rootIndex, "entityOutputPosition", pos);
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
    for(var i = 0, s = display.count; i < s; ++i)
      {
      var item = display.nodeAt(i);
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

    currentInputDragging = component.createObject(display, { firstColour: currentInputDraggingItem.color, lastColour: currentInputDraggingItem.color } );

    var pos = display.mapFromItem(null, x, y);

    currentInputDragging.firstPoint.x = pos.x;
    currentInputDragging.firstPoint.y = pos.y;

    currentInputDragging.lastPoint.x = pos.x;
    currentInputDragging.lastPoint.y = pos.y;

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
    var pos = display.mapFromItem(null, x, y);
    currentInputDragging.lastPoint.x = pos.x;
    currentInputDragging.lastPoint.y = pos.y;
    }

  function endCreatingConnection(x, y)
    {
    currentInputDraggingItem.moveDrag.disconnect(moveCreatingConnection);
    currentInputDraggingItem.endDrag.disconnect(endCreatingConnection);

    var intersectedItemParent = intersect(x, y);

    if(currentInputDraggingItem && intersectedItemParent)
      {
      if(currentInputBeginMode == "input")
        {
        display.changeItemInput(currentInputDraggingItem, intersectedItemParent);
        }
      else
        {
        display.changeItemInput(intersectedItemParent, currentInputDraggingItem);
        }
      }

    currentInputDragging.destroy();
    currentInputDragging = null;
    currentInputDraggingItem = null;
    }

  MouseArea {
    property real lastClickX: 0
    property real lastClickY: 0
    property bool translating: false

    anchors.fill: parent
    onDoubleClicked: {
      nodecanvas.setRootToParent()
      }

    onPressed: {
      if(mouse.modifiers & Qt.AltModifier)
        {
        translating = true;
        }

      lastClickX = mouse.x
      lastClickY = mouse.y
      print("YEAH PRESSED!", mouse.accepted);
    }

    onMousePositionChanged: {
      if(translating)
        {
        var x = mouse.x - lastClickX
        var y = mouse.y - lastClickY

        display.x += x;
        display.y += y;
        }

      lastClickX = mouse.x
      lastClickY = mouse.y
      print("YEAH PRESSED!", mouse.accepted);
      }
    }

  PathHolder {
    x: -2
    y: -2
    nameData: display.path
    model: display.path.length
  }

  NodeDisplay {
    id: display
    model: db
    rootIndex: db.root()

    onNodeAdded: {
      node.z = nodecanvas.maxNodeZ;
    }

    input: Node {
      id: inputs
    }

    output: Node {
      id: outputs
    }

    node: Node {
    }

    connector: Input {
    }
  }
}
