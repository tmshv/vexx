db.addType({
  "name": "SteelPlate",
  "parent": "Component",
  "properties": [
    {
      "name": "RenderGroups",
      "type": "PointerArray",
      "properties": [
        {
          "name": "0",
          "type": "Pointer",
          "defaultInput": "../../3"
        }
      ]
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
      "type": "MCCuboid",
      "properties": [
        {
          "name": "width",
          "extend": true,
          "defaultInput": "../../Size"
        },
        {
          "name": "height",
          "extend": true,
          "defaultInput": "../../Size"
        },
        {
          "name": "depth",
          "extend": true,
          "defaultInput": "../../Thickness"
        }
      ]
    },
    {
      "name": "3",
      "type": "GCGeometryTransform",
      "properties": [
        {
          "name": "geometry",
          "extend": true,
          "defaultInput": "../../1/geometry"
        }
      ]
    }
  ]
});