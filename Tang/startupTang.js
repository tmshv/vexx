script.includeFolder("Components")

db.types.Component.prototype.addInstance = function(shaderGroup)
  {
  shaderGroup.renderGroup.addPointer(this);
  }

// create a new component document and pop it up.
var area = db.addDocument(db.types.AreaDocument);
area.newFile();

var viewportWidget = tang.createViewport();
ui.addDock("Viewport", viewportWidget);

var dbViewport = viewportWidget.viewport();

var cam = dbViewport.addChild(db.types.GCPerspectiveCamera, "Camera");
cam.viewportX.input = vp.x;
cam.viewportY.input = vp.y;
cam.viewportWidth.input = vp.width;
cam.viewportHeight.input = vp.height;

cam.setPosition(0.0, 0.0, 10.0);
cam.setFocalPoint(0.0, 0.0, 0.0);

var msc = vp.addChild(db.types.GCManipulatableScene,"Scene");

msc.setCamera(cam);

//GCShadingGroup *group = msc->addChild<GCShadingGroup>("Groups");
//msc->renderGroup.addPointer(group);

//const SPropertyInformation *standardSurfaceInfo = STypeRegistry::findType("StandardSurface");
//SProperty *shader = msc->addChild(standardSurfaceInfo, "Shader");
//group->shader.setPointed(shader->uncheckedCastTo<GCShader>());

vp.source.setPointed(msc);


// create a new component document and pop it up.
var component = db.addDocument(db.types.ComponentDocument);
component.newFile();


component.loadFile("F:\\CodeVexxLocal\\Tang\\Components\\SteelPlate.json");
var shaderGroup = db.children[1].children.Scene.children.Groups;
component.children[0].addInstance(shaderGroup);

component.editor = component.createEditor();
component.editor.show();

ui.show();
