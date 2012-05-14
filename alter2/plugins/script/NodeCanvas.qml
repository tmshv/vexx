import QtQuick 1.1
import VexxQMLExtensions 1.0
import "NodeCanvasContents"

Rectangle
  {
  id: nodecanvas
  property variant selection: [ ]
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
    clearSelection();
    }

  function drag(item, x, y)
    {
    x /= display.scale;
    y /= display.scale;
    if(!item.selected)
      {
      item.setPosition(item.x + x, item.y + y);
      }
    else
      {
      for(var i = 0, s = nodecanvas.selection.length; i < s; ++i)
        {
        var theItem = nodecanvas.selection[i];
        theItem.setPosition(theItem.x + x, theItem.y + y);
        }
      }
    }

  function dragSelect(x, y, width, height)
    {
    clearSelection();
    for(var i = 0, s = display.count; i < s; ++i)
      {
      var item = display.nodeAt(i);
      if(item.x > x &&
          item.y > y &&
          (item.x+item.width) < x+width &&
          (item.y+item.height) < y+height)
        {
        select(item, false);
        }
      }
    }

  function selectionChanged()
    {
    var sel = [];

    for(var i = 0; i < nodecanvas.selection.length; ++i)
      {
      sel.push(nodecanvas.selection[i].property);
      }

    external.emitRequest("selectionChanged", [ sel ]);
    }

  function clearSelection()
    {
    for(var i = 0; i < nodecanvas.selection.length; ++i)
      {
      nodecanvas.selection[i].selected = false;
      }
    nodecanvas.selection = [ ];
    selectionChanged();
    }

  function select(item, clear)
    {
    if(clear)
      {
      clearSelection();
      }

    var sel = nodecanvas.selection;
    sel.push(item);
    nodecanvas.selection = sel;

    selectionChanged();
    item.selected = true;
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

    var input = display.inputItem;
    if(input.intersect)
      {
      var hit = input.intersect(x, y);
      if(hit)
        {
        return hit;
        }
      }

    var output = display.outputItem;
    if(output.intersect)
      {
      var hit = output.intersect(x, y);
      if(hit)
        {
        return hit;
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
  property variant destroyOnConnectionComplete: null
  property string currentInputBeginMode: ""
  function startCreatingConnection(thing, mode, x, y, colour)
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

    currentInputDragging = component.createObject(display, { firstColour: colour, lastColour: colour } );

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

  function editConnection(conn, mode, x, y)
    {
    display.destroyConnection(conn);
    //destroyOnConnectionComplete = conn;
    //destroyOnConnectionComplete.visible = false;

    /*var output = mode !== "input";

    currentInputDraggingItem = output ? conn.driver : conn.driven;
    currentInputBeginMode = mode;

    conn.moveDrag.connect(moveCreatingConnection);
    conn.endDrag.connect(endCreatingConnection);

    var component = Qt.createComponent("NodeCanvasContents/DynamicInput.qml");

    if(component.status === Component.Error)
      {
      // Error Handling
      console.log("Error loading Input component:", component.errorString());
      return;
      }

    var col = output ? conn.driverColour : conn.drivenColour;
    currentInputDragging = component.createObject(display, { firstColour: col, lastColour: col } );

    var pos = display.mapFromItem(null, x, y);
    var inp = output ? conn.driverPoint : conn.drivenPoint;

    currentInputDragging.firstPoint.x = inp.x;
    currentInputDragging.firstPoint.y = inp.y;

    currentInputDragging.lastPoint.x = pos.x;
    currentInputDragging.lastPoint.y = pos.y;

    if(output)
      {
      currentInputDragging.firstNormal.x = 100
      currentInputDragging.lastNormal.x = -100
      }
    else
      {
      currentInputDragging.firstNormal.x = -100
      currentInputDragging.lastNormal.x = 100
      }*/
    }

  function moveCreatingConnection(x, y)
    {
    var pos = { };
    if(destroyOnConnectionComplete)
      {
      pos = display.mapFromItem(destroyOnConnectionComplete, x, y);
      }
    else
      {
      pos = display.mapFromItem(null, x, y);
      }

    currentInputDragging.lastPoint.x = pos.x;
    currentInputDragging.lastPoint.y = pos.y;
    }

  function endCreatingConnection(x, y)
    {
    if(currentInputDraggingItem.moveDrag)
      {
      currentInputDraggingItem.moveDrag.disconnect(moveCreatingConnection);
      currentInputDraggingItem.endDrag.disconnect(endCreatingConnection);
      }

    if(destroyOnConnectionComplete)
      {
      if(destroyOnConnectionComplete.moveDrag)
        {
        destroyOnConnectionComplete.moveDrag.disconnect(moveCreatingConnection);
        destroyOnConnectionComplete.endDrag.disconnect(endCreatingConnection);
        }

      display.destroyConnection(destroyOnConnectionComplete);
      }
    destroyOnConnectionComplete = null;

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

  function setTopRootIndex(idx)
    {
    display.setTopRootIndex(idx);
    nodecanvas.selection = [ ];
    }


  Component.onCompleted: {
    external.sendRequest.connect(
      function(thing, args){
        nodecanvas[thing].apply(this, args);
      }
    )
  }

  MouseAndScrollArea {
    id: mouseArea

    property real lastClickX: 0
    property real lastClickY: 0
    property bool translating: false
    property bool selecting: false

    acceptedButtons: Qt.LeftButton|Qt.MiddleButton|Qt.RightButton;
    anchors.fill: parent
    onDoubleClicked: {
      nodecanvas.setRootToParent()
      }

    onPressedChanged: {

      if(!pressed)
        {
        if(selecting)
          {
          var mappedMouseA = display.mapFromItem(null, selection.x, selection.y);
          var mappedMouseB = display.mapFromItem(null, selection.x + selection.width, selection.y + selection.height);
          nodecanvas.dragSelect(mappedMouseA.x, mappedMouseA.y, mappedMouseB.x, mappedMouseB.y);
          }
        selecting = false;
        translating = false;
        }
      }

    onScroll: {
      var oldSF = display.scale;
      var sF = Math.max(0.25, Math.min(1.0, display.scale + (delta * 0.00025)));
      display.scale = sF;

      var mult = sF / oldSF;
      var mappedMouse = display.mapFromItem(null, mouse.x, mouse.y);
      display.x += mappedMouse.x - (mappedMouse.x * mult);
      display.y += mappedMouse.y - (mappedMouse.y * mult);
    }

    onPressed: {
      lastClickX = mouse.x;
      lastClickY = mouse.y;

      if(mouse.button === Qt.RightButton)
        {
        external.emitRequest("contextMenu", [ mouse.x, mouse.y, display.path ]);
        mouse.accepted = true;
        return;
        }

      if(((mouse.modifiers & Qt.AltModifier) !== 0 && mouse.button === Qt.LeftButton))
        {
        translating = true;
        mouse.accepted = true;
        return;
        }
      else if(mouse.modifiers === 0 && mouse.button === Qt.MiddleButton)
        {
        translating = true;
        mouse.accepted = true;
        return;
        }

      if(mouse.modifiers === 0 && mouse.button === Qt.LeftButton)
        {
        selecting = true;
        selection.x = mouse.x;
        selection.y = mouse.y;
        selection.width = 0;
        selection.height = 0;
        mouse.accepted = true;
        return;
        }
      }

    onMousePositionChanged: {
      if(selecting)
        {
        if(selection.moveLeft)
          {
          selection.width += selection.x - mouse.x;
          selection.x = mouse.x;
          }
        else
          {
          selection.width = mouse.x - selection.x;
          }

        if(selection.moveTop)
          {
          selection.height += selection.y - mouse.y;
          selection.y = mouse.y;
          }
        else
          {
          selection.height = mouse.y - selection.y;
          }

        if(selection.width < 0)
          {
          selection.moveLeft ^= true;
          selection.x += selection.width;
          selection.width = Math.abs(selection.width);
          }

        if(selection.height < 0)
          {
          selection.moveTop ^= true;
          selection.y += selection.height;
          selection.height = Math.abs(selection.height);
          }
        }
      else if(translating)
        {
        var dx = mouse.x - lastClickX
        var dy = mouse.y - lastClickY

        display.x += dx;
        display.y += dy;
        }
      lastClickX = mouse.x
      lastClickY = mouse.y
      }
    }

  Rectangle {
    id: selection
    property bool moveTop: false
    property bool moveLeft: false

    x: 100
    y: 100
    width: 200
    height: 200

    z: 2

    visible: mouseArea.selecting

    smooth: true
    radius: 2
    color: "#551F9AFF"

    border.width: 2
    border.color: "#007BE0"
  }

  PathHolder {
    x: -2
    y: -2
    z: 3
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
