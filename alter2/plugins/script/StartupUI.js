function doContextMenu(window, x, y)
  {
  var menuContents =
    {
    "abc": {
      description: "Wow, this is cool",
      request: "gogo",
      requestData: [ "C" ]
      },
    "def": {
      description: "Wow, this is cool too",
      children: {
        "stub1": {
          description: "1",
          request: "gogo1",
          requestData: [ "A" ]
          },
        "stub2": {
          description: "2",
          request: "gogo2",
          requestData: [ "B" ]
          }
        }
      }
    }

  var contextMenu =
    {
    surface: script.addQMLWindow("../alter2/plugins/script/NodeCanvasContextMenu.qml",
                                 { transparent: true, tool: true, focusPolicy: "strong", menuContents: menuContents }),
    passIn: function(name, argsIn)
      {
      assert(this[name]);
      this[name].apply(this, argsIn);
      },
    gogo: function(data)
          {
            print("A", data);
          },
    gogo1: function(data)
          {
            print("B", data);
          },
    gogo2: function(data)
          {
            print("C", data);
          }
    }
  contextMenu.surface.emitRequest.connect(contextMenu, contextMenu.passIn);
  var mapped = window.mapTo(null, x, y);
  contextMenu.surface.setPosition(mapped.x, mapped.y);
  contextMenu.surface.show();
  }

setupDebugSurface = function()
  {
  var surfaceManager =
    {
    surface: script.addQMLSurface("DebugQMLCanvas", "Dock", "../alter2/plugins/script/NodeCanvas.qml"),
    passIn: function(name, argsIn)
      {
      assert(this[name]);
      this[name].apply(this, argsIn);
      },
    contextMenu: function(x, y)
      {
      doContextMenu(this.surface, x, y);
      }
    }
  surfaceManager.surface.emitRequest.connect(surfaceManager, surfaceManager.passIn);
  }

setupDebugSurface();
