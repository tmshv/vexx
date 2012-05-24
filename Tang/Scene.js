
function Scene(viewportWidget)
  {
  var dbViewport = viewportWidget.viewport();

  var cam = dbViewport.addChild(db.types.GCPerspectiveCamera, "Camera");
  cam.viewportX.input = dbViewport.x;
  cam.viewportY.input = dbViewport.y;
  cam.viewportWidth.input = dbViewport.width;
  cam.viewportHeight.input = dbViewport.height;

  cam.setPosition(0.0, 0.0, 10.0);
  cam.setFocalPoint(0.0, 0.0, 0.0);


  this.scene = dbViewport.addChild(db.types.GCManipulatableScene,"Scene");

  this.scene.setCamera(cam);
  viewportWidget.setScene(this.scene);

  this.updateSelection = false;

  function nameChange(prop, before, after, backwards) { }
  function treeChange(prop, before, after, backwards)
  {
    var selection = this.scene.selection;
    if((before && before.equals(selection)) || (after && after.equals(selection)))
    {
      this.updateSelection = true;
    }
  }

  function finalise()
  {
    if(this.updateSelection)
    {
      this.updateSelection = false;

      var selectedItems = [];

      var selection = this.scene.selection;
      for(var i = 0; i < selection.length; ++i)
      {
        var selected = selection[i].input;
        if(selected)
        {
          selectedItems.push(selected);
        }
      }
    }
  }

  this.selectionObsserver = new STreeObserver(this, treeChange, nameChange, finalise);
  this.scene.addTreeObserver(this.selectionObsserver);
  }


Scene.prototype.setViewed = function(renderable)
  {
  tang.mainScene.scene.renderGroup.clear();
  renderable.addInstanceTo(tang.mainScene.scene);
  }
