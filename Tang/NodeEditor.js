script.include("ViewportContextMenu.js")

function NodeEditor(topLevel)
  {
  this.surface = script.addQMLSurface("NodeEditor", "Dock", "../alter2/plugins/script/NodeCanvas.qml", {});

  var passIn = function(name, argsIn)
    {
    assert(this[name]);
    this[name].apply(this, argsIn);
    };

  this.contextMenu = function(x, y, path)
    {
    popupViewportContextMenu(new Point(x, y), this.surface, path);
    }

  this.surface.emitRequest.connect(this, passIn);

  this.surface.sendRequest("setTopRootIndex", [ topLevel ]);
  }


