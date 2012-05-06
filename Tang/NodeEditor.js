script.include("ViewportContextMenu.js")

setupNodeEditor = function(topLevel)
  {
  var surfaceManager =
    {
    surface: script.addQMLSurface("NodeEditor", "Dock", "../alter2/plugins/script/NodeCanvas.qml", {}),
    passIn: function(name, argsIn)
      {
      assert(this[name]);
      this[name].apply(this, argsIn);
      },
    contextMenu: function(x, y, path)
      {
      popupViewportContextMenu(new Point(x, y), this.surface, path);
      }
    }
  surfaceManager.surface.emitRequest.connect(surfaceManager, surfaceManager.passIn);

  surfaceManager.surface.sendRequest("setTopRootIndex", [ topLevel ]);
  }


