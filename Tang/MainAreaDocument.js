function MainAreaDocument()
  {
  var newObj = this;
  // create a new component document and pop it up.
  newObj.document = db.addDocument(db.types.AreaDocument);

  mainAreaDocumentNewFileBuilder = function(objToAddTo)
    {
    var document = objToAddTo.document;
    var oldFn = document.newFile;
    return function()
      {
      document.undoBlock(
        function()
          {
          oldFn.apply(document);

          // get the area from the doc
          objToAddTo.area = document.children[0];
          var shadingGroup = objToAddTo.area.shaderGroups.add(db.types.GCShadingGroup, "Groups");

          var shader = objToAddTo.area.shaders.add(db.types.StandardSurface, "Shader");
          shadingGroup.shader.input = shader;
        });
      }
    }

  newObj.document.newFile = mainAreaDocumentNewFileBuilder(newObj);
  newObj.document.newFile();

  newObj.addRenderables = function(renderer)
    {
    for(var i=0; i< newObj.area.shaderGroups.length; ++i)
      {
      renderer.addInstance(newObj.area.shaderGroups[i]);
      }
    }
  }

var a = null;
