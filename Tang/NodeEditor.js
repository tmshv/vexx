script.include("ViewportContextMenu.js")

function NodeEditor()
  {
  this.surface = script.addQMLSurface("NodeEditor", "Dock", "../alter2/plugins/script/NodeCanvas.qml", {});

  var passIn = function(name, argsIn)
    {
    assert(this[name]);
    this[name].apply(this, argsIn);
    };

  this.surface.emitRequest.connect(this, passIn);
  }

NodeEditor.prototype.selectionChanged = function(selection)
  {
  var selectionArray = tang.mainScene.scene.selection;
  selectionArray.clear();

  for(var i = 0; i < selection.length; ++i)
    {
    selectionArray.addPointer(selection[i]);
    }
  }

NodeEditor.prototype.contextMenu = function(x, y, path)
  {
  popupViewportContextMenu(new Point(x, y), this.surface, path);
  }

NodeEditor.prototype.setViewed = function(topLevel)
  {
  this.surface.sendRequest("setTopRootIndex", [ topLevel.renderable ]);
  }
