db.addType({
  "name": "SteelPlate",
  "parent": "Component",
  "properties": [
    {
      "name": "__position",
      "type": "Vector3DProperty",
      "defaultValue": "554 -36 0"
    },
    {
      "name": "__outputsPosition",
      "type": "Vector3DProperty",
      "defaultValue": "758 49 0"
    },
    {
      "name": "__inputsPosition",
      "type": "Vector3DProperty",
      "defaultValue": "-132 31 0"
    },
    {
      "name": "RenderGroups",
      "type": "PointerArray"
    },
    {
      "name": "Thickness",
      "type": "FloatProperty",
      "defaultValue": "0"
    },
    {
      "name": "Size",
      "type": "FloatProperty",
      "defaultValue": "0"
    },
    {
      "name": "1",
      "type": "MCCuboid"
    },
    {
      "name": "3",
      "type": "GCGeometryTransform"
    }
  ]
});