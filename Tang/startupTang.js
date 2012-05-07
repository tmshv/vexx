script.includeFolder("Components")
script.include("ViewportContextMenu.js")

db.types.GCRenderArray.prototype.addInstance = function(renderable)
  {
  this.renderGroup.addPointer(renderable);
  }


var viewportWidget = tang.createViewport();
viewportWidget.setContextMenuHandler(viewportWidget, function(pos) { popupViewportContextMenu(pos, this) });
ui.addDock("Viewport", viewportWidget);

tang.mainViewport = viewportWidget;

script.include("Scene.js");
tang.mainScene = new Scene(viewportWidget);

script.include("MainAreaDocument.js");
tang.mainAreaDocument = new MainAreaDocument();

print(tang.mainAreaDocument.newFile);

tang.mainAreaDocument.addRenderables(tang.mainScene);

script.include("NodeEditor.js");
tang.nodeEditor = NodeEditor(tang.mainAreaDocument.area);


// create a new component document and pop it up.
//var componentDocument = db.addDocument(db.types.ComponentDocument);
//componentDocument.newFile();

//componentDocument.loadFile("..\\Tang\\Components\\SteelPlate.json");

/*var component = componentDocument.children[0];
component.addInstance(shadingGroup);*/

//componentDocument.editor = componentDocument.createEditor();
//componentDocument.editor.show();


ui.show();
