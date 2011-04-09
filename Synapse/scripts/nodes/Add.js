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
        // something like: this.Output.add(this.InputA, this.InputB);
        }
      }
    ]
  });
