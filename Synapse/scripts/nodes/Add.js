db.addType({
  name: "Add",
  parent: "SyNode",
  properties:
    [
      {
      name: "InputA",
      type: "SyImageInput",
      affects: [ "Output" ]
      },
      {
      name: "InputB",
      type: "SyImageInput",
      affects: [ "Output" ]
      },
      {
      name: "Output",
      type: "SyImageOutput",
      compute: function()
        {
        this.Output.image.add(this.InputA.image, this.InputB.image);
        }
      }
    ]
  });
