script.includeFolder("Components")
script.include("ViewportContextMenu.js")

db.types.GCRenderable.prototype.addInstanceTo = function(renderArray)
  {
  renderArray.renderGroup.addPointer(this);
  }

var viewportWidget = tang.createViewport();
viewportWidget.setContextMenuHandler(viewportWidget, function(pos) { popupViewportContextMenu(pos, this) });
ui.addDock("Viewport", viewportWidget);

tang.mainViewport = viewportWidget;

script.include("Scene.js");
tang.mainScene = new Scene(viewportWidget);

script.include("MainAreaDocument.js");
tang.mainAreaDocument = new MainAreaDocument();

script.include("NodeEditor.js");
tang.nodeEditor = new NodeEditor();
tang.mainAreaDocument.addViewer(tang.nodeEditor);
tang.mainAreaDocument.addViewer(tang.mainScene);


// create a new component document and pop it up.
//var componentDocument = db.addDocument(db.types.ComponentDocument);
//componentDocument.newFile();

//componentDocument.loadFile("..\\Tang\\Components\\SteelPlate.json");

/*var component = componentDocument.children[0];
component.addInstance(shadingGroup);*/

//componentDocument.editor = componentDocument.createEditor();
//componentDocument.editor.show();

var obj = { };

function treeChange()
{
  print("treeChange", obj === this)
}
function finalise()
{
  print("finalise", obj === this)
}

obs = new STreeObserver(obj, treeChange, finalise);
tang.mainScene.scene.addTreeObserver(obs);

ui.show();
