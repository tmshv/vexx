script.includeFolder("Components")

db.types.Component.prototype.addInstance = function(shaderGroup)
  {
  shaderGroup.renderGroup.addPointer(this);
  }

// create a new component document and pop it up.
var document = db.addDocument(db.types.ComponentDocument);
document.newFile();

document.editor = document.createEditor();
document.editor.show();

//var shaderGroup = db.children[1].children.Scene.children.Groups
//db.children[2].children.Component1.addInstance(shaderGroup)
