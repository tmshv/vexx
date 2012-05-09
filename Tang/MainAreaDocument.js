function MainAreaDocument()
  {
  var newObj = this;
  // create a new component document and pop it up.
  newObj.document = db.addDocument(db.types.AreaDocument);

  newObj.viewers = [];
  newObj.renderable = null;

  var updateInternal = function(objToAddTo)
  {
  // get the area from the doc
  objToAddTo.area = objToAddTo.document.children[0];
  objToAddTo.renderable = objToAddTo.area;

  for(var i=0; i<objToAddTo.viewers.length; ++i)
    {
    objToAddTo.viewers[i].setViewed(objToAddTo);
    }
  }

  mainAreaDocumentNewFileBuilder = function(objToAddTo)
    {
    var document = objToAddTo.document;
    var oldFn = document.newFile;
    return function()
      {
      document.undoBlock(
        function()
          {
          oldFn.apply(document, arguments);

          // get the area from the doc
          var area = document.children[0];

          var shadingGroup = area.shaderGroups.add(db.types.GCShadingGroup, "Groups");
          var shader = area.shaders.add(db.types.StandardSurface, "Shader");
          shadingGroup.shader.input = shader;

          updateInternal(objToAddTo);
        });
      }
    }

  newObj.document.newFile = mainAreaDocumentNewFileBuilder(newObj);


  mainAreaDocumentLoadFileBuilder = function(objToAddTo)
    {
    var document = objToAddTo.document;
    var oldFn = document.loadFile;
    return function()
      {
      document.undoBlock(
        function()
          {
          oldFn.apply(document, arguments);

          updateInternal(objToAddTo);
        });
      }
    }

  newObj.document.loadFile = mainAreaDocumentLoadFileBuilder(newObj);

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

