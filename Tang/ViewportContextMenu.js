function popupViewportContextMenu(pos, window)
  {

  function buildTypeData()
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

    var typesData = [
      {
        name: "Utilities",
        children: [
          {
            name: "Compose Vector3",
            request: "create",
            requestData: [ "GCComposeVector3" ]
          },
          {
            name: "Compose Transform",
            request: "create",
            requestData: [ "GCComposeTransform" ]
          },
          {
            name: "Multiply",
            request: "create",
            requestData: [ "MCSimpleMultiply" ]
          },
          {
            name: "Add",
            request: "create",
            requestData: [ "MCSimpleAdd" ]
          }
        ]
      },
      {
        name: "Cube",
        request: "instanceComponent",
        requestData: [ "MCCuboid" ]
      },
      {
        name: "Transform",
        request: "instanceComponent",
        requestData: [ "GCTransform" ]
      }
    ];

    for(name in db.types)
    {
      var type = db.types[name];
      if(isComponent(type))
      {
        typesData.push({
          name: type.typeName,
          request: "instanceComponent",
          requestData: [ type.typeName ]
          });
      }
    }

    return typesData;
  }

  function buildRecentFiles()
    {
    var fileData = [
    ];

    var fileList = ui.recentFileList();
    for(var i=0; i<fileList.length; ++i)
    {
      var file = fileList[i];
      fileData.push({
        name: file,
        request: "loadRecentFile",
        requestData: [ file ]
        });
    }

    return fileData;
  }

  var menuContents =
    [
      {
      name: "Create",
      description: "Create an object",
      children: buildTypeData()
      },
      {
      name: "New File",
      request: "newFile"
      },
      {
      name: "Load File",
      request: "loadFile"
      },
      {
      name: "Load Recent File",
      children: buildRecentFiles()
      },
      {
      name: "Save File",
      request: "saveFile"
      },
      {
      name: "Save File As",
      request: "saveFileAs"
      }
    ];

  var surface = script.addQMLWindow("../alter2/plugins/script/NodeCanvasContextMenu.qml",
                                    { transparent: true, tool: true, focusPolicy: "strong", menuContents: menuContents });

  var contextMenu =
    {
    instanceComponent: function(data)
      {
      if(tang.mainAreaDocument.area.shaderGroups.length)
        {
        var shadingGroup = tang.mainAreaDocument.area.shaderGroups[0];
        var comp = tang.mainAreaDocument.area.addChild(data, "NewComponent");
        comp.addInstanceTo(shadingGroup);
        }
      },
    create: function(data)
      {
      tang.mainAreaDocument.area.addChild(data, data);
      },
    newFile: function()
      {
      tang.mainAreaDocument.document.newFile();
      },
    saveFile: function()
      {
      var filename = tang.mainAreaDocument.document.filename.value;
      if(filename === "")
        {
        filename = ui.getSaveFilename("Area File (*.jsarea)");
        }

      if(filename !== "")
        {
        tang.mainAreaDocument.document.saveFile(filename);
        }
      else
        {
        print("Save Cancelled");
        }
      },
    saveFileAs: function()
      {
      var filename = ui.getSaveFilename("Area File (*.jsarea)");
      if(filename !== "")
        {
        tang.mainAreaDocument.document.saveFile(filename);
        }
      else
        {
        print("Save Cancelled");
        }
      },
    loadFile: function()
      {
      var filename = ui.getOpenFilename("Area File (*.jsarea)");
      if(filename !== "")
        {
        tang.mainAreaDocument.document.loadFile(filename);
        }
      else
        {
        print("Load Cancelled");
        }
      },
    loadRecentFile: function(filename)
      {
      if(filename !== "")
        {
        tang.mainAreaDocument.document.loadFile(filename);
        }
      else
        {
        print("Cant Load Empty file");
        }
      }
    }

  var toFire = "";
  var toFireArgs = { };

  var passIn = function(name, argsIn)
    {
    toFire = name;
    toFireArgs = argsIn;
    }

  var passInTrigger = function()
    {
    if(contextMenu[toFire])
      {
      contextMenu[toFire].apply(contextMenu, toFireArgs);
      }
    }
  surface.emitRequest.connect(passIn);
  surface.destroyed.connect(passInTrigger);

  var mapped = window.mapToGlobal(pos);
  surface.setPosition(mapped.x, mapped.y);
  surface.show();
  }

