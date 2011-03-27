db.addType({
  name: "Add",
  parent: "SyNode",
  properties:
    [
      {
      name: "InputA",
      type: "IntProperty",
      affects: [ "Output", "Output2" ]
      },
      {
      name: "InputB",
      type: "IntProperty",
      affects: [ "Output", "Output2" ]
      },
      {
      name: "Output",
      type: "IntProperty",
      compute: function()
        {
        this.Output.setValue(this.InputA.value() + this.InputB.value());
        }
      },
      {
      name: "Output2",
      type: "IntProperty",
      compute: function()
        {
        this.Output2.setValue(this.InputA.value() * this.InputB.value());
        }
      }
    ]
  });
