function assert(exp)
  {
  if(!exp)
    {
    script.enableDebugging(true);

    throw arguments.length > 1 ? arguments[1] : "Assertion failed";
    return false;
    }
  return true;
  }


/*

// example node
db.addType({
  name: "NodeName",
  parent: SEntity,
  properties:
    [
      {
      name: "AttrName",
      type: FloatProperty,
      affects: [ "Output" ]
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
    ]
  });

  */

script.include("JSPartExporter.js")
