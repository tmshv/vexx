import QtQuick 1.1
import VexxQMLExtensions 1.0

Path {
  id: inputHolder
  property variant myIndex: null
  property variant inputIndex: db.data(myIndex, "propertyInput");
  property variant myProperty: null
  property variant inputTarget: null

  width: 4

  firstNormal.x: 100
  firstNormal.y: 0
  lastNormal.x: -100
  lastNormal.y: 0

  function propertyChanged(prop)
    {
    // should i have to disconnect signals manually? i hope not, but reference errors otherwise,
    // test on 4.8 non-RC
    myProperty.propertyChanged.disconnect(inputHolder.propertyChanged);
    if(inputTarget && inputTarget.propertyChanged)
    {
      inputTarget.propertyChanged.disconnect(inputHolder.propertyChanged);
    }

    var item = myProperty;
    while(item)
      {
      item.xChanged.disconnect(inputHolder.update);
      item.yChanged.disconnect(inputHolder.update);
      item = item.parent;
      }

    item = inputTarget;
    while(item)
      {
      item.xChanged.disconnect(inputHolder.update);
      item.yChanged.disconnect(inputHolder.update);
      item = item.parent;
      }

    // re set this up, as our output property has been destroyed.
    if(prop === myProperty)
      {
      myProperty.input = null;
      inputHolder.destroy();
      }
    else
      {
      recreateTimer.start();
      //visible = false;
      }
    }

  function update()
    {
    inputHolder.firstPoint = inputTarget.getOutputPosition(parent);
    inputHolder.lastPoint = myProperty.getInputPosition(parent);
    }

  function setupInput()
    {
    setupTimer.start();
    }

  Timer {
    id: recreateTimer
    interval: 1
    onTriggered: {
      if(myProperty)
        {
        var index = myProperty.getModelIndex();
        if(db.isValid(index))
          {
          var items = nodecanvas.findPropertyItem(index, "input");
          if(items)
            {
            nodecanvas.setupInput(items[items.length-1], index);
            }
          }
        myProperty.input = null;
        }

      inputHolder.destroy();
    }
  }

  Timer {
    id: resetupTimer
    interval: 100
    onTriggered: {
      print("Resetup!");
      setupTimer.start();
    }
  }

  Timer {
    id: setupTimer
    interval: 1
    onTriggered: {
      if(!db.isValid(inputIndex))
        {
        print("Pants, input is invalid, hide! SHOULDN'T REACH THIS");
        inputHolder.destroy();
        return;
        }

      var items = nodecanvas.findPropertyItem(myIndex, "input");
      if(!items)
        {
        print("Iitem is not available, this is way bad.");
        inputHolder.destroy();
        return;
        }
      for(var i = 0, s = items.length; i < s; ++i)
        {
        var item = items[i];
        item.xChanged.connect(inputHolder.update);
        item.yChanged.connect(inputHolder.update);
        }


      var oItems = nodecanvas.findPropertyItem(inputIndex, "output");
      if(!oItems)
        {
        print("Input item is not available");
        inputHolder.destroy();
        return;
        }

      for(var i = 0, s = oItems.length; i < s; ++i)
        {
        var item = oItems[i];
        item.xChanged.connect(inputHolder.update);
        item.yChanged.connect(inputHolder.update);
        }

      inputTarget = oItems[oItems.length-1];
      if(!inputTarget)
        {
        print("cant find the item with the input???");
        }

      if(inputTarget.propertyChanged)
        {
        inputTarget.propertyChanged.connect(propertyChanged);
        }

      if(myProperty.propertyChanged)
        {
        myProperty.propertyChanged.connect(propertyChanged);
        }

      //inputHolder.visible = (function() { return myProperty.visible && inputTarget.visible });
      inputHolder.firstColour = (function() { return inputTarget.colour; });
      inputHolder.lastColour = (function() { return myProperty.colour; });

      update();
    }
  }
}
