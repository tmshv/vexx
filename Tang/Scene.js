
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
  }


Scene.prototype.setViewed = function(renderable)
  {
  tang.mainScene.scene.renderGroup.clear();
  renderable.addInstanceTo(tang.mainScene.scene);
  }
