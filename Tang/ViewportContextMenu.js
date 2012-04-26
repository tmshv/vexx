function popupViewportContextMenu(pos, window)
  {
  isComponent = function(obj)
    {
    if(!obj.prototype)
      {
      return false;
      }
    var proto = obj.prototype.__proto__;

    while(proto)
      {
      if(proto.typeName === db.types.Component.typeName)
        {
        return true;
        }

      if(!proto.prototype)
        {
        break;
        }

      proto = proto.prototype.__proto__;
      }
    return false;
    }

  var typesData = {
    "Cube": {
      description: "",
      request: "instanceComponent",
      requestData: [ "MCCuboid" ]
    }
  };
  for(name in db.types)
    {
    var type = db.types[name];
    if(isComponent(type))
      {
      typesData[type.typeName] = {
        description: "",
        request: "instanceComponent",
        requestData: [ type.typeName ]
        }
      }
    }

  var menuContents =
    {
    "Create": {
      description: "Instance a Component",
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
    instanceComponent: function(data)
      {
      if(tang.mainArea.shaderGroups.length)
        {
        var shadingGroup = tang.mainArea.shaderGroups[0];
        var comp = tang.mainArea.addChild(data, "NewComponent");
        comp.addInstance(shadingGroup);
        }
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
  var mapped = window.mapTo(null, pos);
  contextMenu.surface.setPosition(mapped.x, mapped.y);
  contextMenu.surface.show();
  }
