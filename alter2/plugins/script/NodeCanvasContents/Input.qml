import QtQuick 1.1

Rectangle {
  id: inputHolder
  property variant myIndex: null
  property variant inputIndex: db.data(myIndex, "propertyInput");
  property variant myProperty: null
  property variant inputTarget: null

  function propertyChanged(prop)
    {
    // should i have to disconnect signals manually? i hope not, but reference errors otherwise,
    // test on 4.8 non-RC
    myProperty.propertyChanged.disconnect(inputHolder.propertyChanged);
    if(inputTarget.propertyChanged)
    {
      inputTarget.propertyChanged.disconnect(inputHolder.propertyChanged);
    }

    var items = nodecanvas.findPropertyItem(inputIndex);
    for(var i = 1, s = items.length; i < s; ++i)
      {
      var item = items[i];
      item.xChanged.disconnect(inputHolder.update);
      item.yChanged.disconnect(inputHolder.update);
      }

    items = nodecanvas.findPropertyItem(myIndex);
    for(var i = 1, s = items.length; i < s; ++i)
      {
      var item = items[i];
      item.xChanged.disconnect(inputHolder.update);
      item.yChanged.disconnect(inputHolder.update);
      }

    // re set this up, as our output property has been destroyed.
    if(prop != myProperty)
      {
      nodecanvas.setupInput(myProperty, myIndex);
      }

    inputHolder.destroy();
    }

  color: "red"
  function update()
    {
    var from = inputTarget.getOutputPosition(parent);
    var to = myProperty.getInputPosition(parent);

    inputHolder.x = from.x;
    inputHolder.y = from.y;

    inputHolder.width = -from.x + to.x;
    inputHolder.height = -from.y + to.y;
    }

  function setupInput()
    {
    setupTimer.start();
    }

  Timer {
    id: setupTimer
    interval: 1
    onTriggered: {
      if(!inputIndex)
        {
        print("Pants, input is invalid, hide!");
        visible = false;
        return;
        }

      var items = nodecanvas.findPropertyItem(inputIndex);
      if(!items)
        {
        print("Input item is not available");
        visible = false;
        return;
        }

      visible = true;
      for(var i = 1, s = items.length; i < s; ++i)
        {
        var item = items[i];
        item.xChanged.connect(inputHolder.update);
        item.yChanged.connect(inputHolder.update);
        }

      inputTarget = items[items.length-1];
      if(!inputTarget)
        {
        print("cant find the item with the input???");
        }

      if(inputTarget.propertyChanged)
        {
        inputTarget.propertyChanged.connect(propertyChanged);
        }

      items = nodecanvas.findPropertyItem(myIndex);
      for(var i = 1, s = items.length; i < s; ++i)
        {
        var item = items[i];
        item.xChanged.connect(inputHolder.update);
        item.yChanged.connect(inputHolder.update);
        }


      if(myProperty.propertyChanged)
        {
        myProperty.propertyChanged.connect(propertyChanged);
        }

      update();
    }
  }
}
