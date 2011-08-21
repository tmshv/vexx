db.addType({
  name: "Project",
  parent: SEntity,
  properties:
    [
      {
      name: "ProjectDependancies",
      type: PointerArray
      },
      {
      name: "AttrName2",
      type: IntProperty,
      affects: [ "Output" ]
      },
      {
      name: "Output",
      type: FloatProperty,
      compute: function()
        {
        this.Output = this.AttrName * this.AttrName2;
        }
      }
    ],
  prototype:
    {
      open: function(file)
        {
             print("open " + file);
        }
    }
  });
