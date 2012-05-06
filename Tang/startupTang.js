script.includeFolder("Components")
script.include("ViewportContextMenu.js")
script.include("NodeEditor.js")

db.types.Component.prototype.addInstance = function(shaderGroup)
  {
  shaderGroup.renderGroup.addPointer(this);
  }


var viewportWidget = tang.createViewport();
viewportWidget.setContextMenuHandler(viewportWidget, function(pos) { popupViewportContextMenu(pos, this) });
ui.addDock("Viewport", viewportWidget);

tang.mainViewport = viewportWidget;

var createBasicScene = function(viewportWidget)
  {
  var dbViewport = viewportWidget.viewport();

  var cam = dbViewport.addChild(db.types.GCPerspectiveCamera, "Camera");
  cam.viewportX.input = dbViewport.x;
  cam.viewportY.input = dbViewport.y;
  cam.viewportWidth.input = dbViewport.width;
  cam.viewportHeight.input = dbViewport.height;

  cam.setPosition(0.0, 0.0, 10.0);
  cam.setFocalPoint(0.0, 0.0, 0.0);

  var msc = dbViewport.addChild(db.types.GCManipulatableScene,"Scene");

  msc.setCamera(cam);
  viewportWidget.setScene(msc);

  return msc;
  }

tang.mainScene = createBasicScene(viewportWidget);


// create a new component document and pop it up.
tang.mainAreaDocument = db.addDocument(db.types.AreaDocument);
tang.mainAreaDocument.newFile();

// get the area from the doc
tang.mainArea = tang.mainAreaDocument.children[0];
var shadingGroup = tang.mainArea.shaderGroups.add(db.types.GCShadingGroup, "Groups");
tang.mainScene.renderGroup.addPointer(shadingGroup);

var shader = tang.mainArea.shaders.add(db.types.StandardSurface, "Shader");
shadingGroup.shader.input = shader;


setupNodeEditor(tang.mainArea);


// create a new component document and pop it up.
//var componentDocument = db.addDocument(db.types.ComponentDocument);
//componentDocument.newFile();

//componentDocument.loadFile("..\\Tang\\Components\\SteelPlate.json");

/*var component = componentDocument.children[0];
component.addInstance(shadingGroup);*/

//componentDocument.editor = componentDocument.createEditor();
//componentDocument.editor.show();


ui.show();
