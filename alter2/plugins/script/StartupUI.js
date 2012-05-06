var userCreatableEntities = [ ];

function doContextMenu(window, x, y, path)
  {
  var typesData = [ ];
  for(var i = 0; i < userCreatableEntities.length; ++i)
    {
    var type = userCreatableEntities[i];
    typesData.push({
      name: type.typeName,
      description: "Some Type",
      request: "create",
      requestData: [ type.typeName ]
      });
    }

  var menuContents =
    [
      {
      name: "Create",
      description: "Add new nodes",
      children: typesData
      }
    ];

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
      var parent = db;
      for(var i = 1; i < path.length; ++i)
        {
        parent = parent.children[path[i]];
        if(!parent)
          {
          throw "Couldn't find parent " + path[i] + "to create child under";
          }
        }

      parent.addChild(data, data);
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
    surface: script.addQMLSurface("DebugQMLCanvas", "Dock", "../alter2/plugins/script/NodeCanvas.qml", {}),
    passIn: function(name, argsIn)
      {
      assert(this[name]);
      this[name].apply(this, argsIn);
      },
    contextMenu: function(x, y, path)
      {
      doContextMenu(this.surface, x, y, path);
      }
    }
  surfaceManager.surface.emitRequest.connect(surfaceManager, surfaceManager.passIn);
  }

setupDebugSurface();
