db.addType({
  name: "Project",
  parent: SEntity,
  properties:
    [
      {
      name: "rootDirectory",
      type: StringProperty
      },
      {
      name: "projectDependancies",
      type: PointerArray
      }
    ],
  prototype:
    {
    create: function(directory, name)
      {
      var pro = db.addChild(Project, name);
      pro.rootDirectory = directory;

      return pro;
      },
    save: function()
      {
      print(db.save("json", this));
      }
    }
  });
