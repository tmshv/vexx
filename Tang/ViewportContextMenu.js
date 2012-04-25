function popupViewportContextMenu(pos, window)
  {
  inherits = function(obj, parent)
    {
    var proto = obj;
    while(proto)
      {
      if(proto.typeName === parent.typeName)
        {
        print(proto.typeName)
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
      request: "create",
      requestData: [ "MCCuboid" ]
    }
  };
  for(name in db.types)
    {
    var type = db.types[name];
    if(inherits(type, db.types.Component))
      {
      typesData[type.typeName] = {
        description: "",
        request: "create",
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
