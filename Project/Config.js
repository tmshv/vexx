db.addType({
  name: "Config",
  parent: SEntity,
  properties:
    [
      {
      name: "rootDirectory",
      type: StringProperty
      }
    ],
  prototype:
    {
    create: function(name)
      {
      print("Creating config '" + name + "'");
      var pro = db.addChild(Config, name);
      return pro;
      }
    }
  });
