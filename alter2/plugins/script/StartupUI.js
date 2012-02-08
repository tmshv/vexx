var groups = [
  db.types.MCImage
]

function doContextMenu(window, x, y)
  {
  var typesData = { };
  for(var i = 0; i < groups.length; ++i)
    {
    var type = groups[i];
    typesData[type.typeName] = {
      description: "Some Type",
      request: "create",
      requestData: [ type.typeName ]
      }
    }

  var menuContents =
    {
    "Create": {
      description: "Add new nodes",
      children: typesData
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
    create: function(data)
      {
      db.addChild(data, data);
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