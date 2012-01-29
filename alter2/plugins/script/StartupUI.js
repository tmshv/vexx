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
                   var window = script.addQMLWindow("../alter2/plugins/script/NodeCanvasContextMenu.qml", { transparent: true, tool: true, focusPolicy: "strong" });
      var mapped = this.surface.mapTo(null, x, y);
      window.setPosition(mapped.x, mapped.y);
      window.show();
      }
    }
  surfaceManager.surface.emitRequest.connect(surfaceManager, surfaceManager.passIn);
  }

setupDebugSurface();
