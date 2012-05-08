function MainAreaDocument()
  {
  var newObj = this;
  // create a new component document and pop it up.
  newObj.document = db.addDocument(db.types.AreaDocument);

  newObj.viewers = [];
  newObj.renderable = null;

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
          objToAddTo.renderable = objToAddTo.area;
          var shadingGroup = objToAddTo.area.shaderGroups.add(db.types.GCShadingGroup, "Groups");

          var shader = objToAddTo.area.shaders.add(db.types.StandardSurface, "Shader");
          shadingGroup.shader.input = shader;

          for(var i=0; i<objToAddTo.viewers.length; ++i)
            {
            objToAddTo.viewers[i].setViewed(objToAddTo);
            }
        });
      }
    }

  newObj.document.newFile = mainAreaDocumentNewFileBuilder(newObj);
  newObj.document.newFile();
  }

MainAreaDocument.prototype.addViewer = function(viewer)
  {
  this.viewers.push(viewer);
  viewer.setViewed(this);
  }

MainAreaDocument.prototype.addInstanceTo = function(renderer)
  {
  for(var i=0; i<this.area.shaderGroups.length; ++i)
    {
    this.area.shaderGroups[i].addInstanceTo(renderer);
    }
  }

