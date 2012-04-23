db.types.registerType({
  "name": "SteelPlate",
  "parent": "Component",
  "properties": [
    {
      "name": "renderGroup",
      "extend": true,
      "properties": [
        {
          "name": "1",
          "type": "GCRenderablePointer",
          "defaultInput": "../../undefined"
        }
      ]
    },
    {
      "name": "Thickness",
      "type": "FloatProperty",
      "defaultValue": "0.01"
    },
    {
      "name": "Width",
      "type": "FloatProperty",
      "defaultValue": "1.2"
    },
    {
      "name": "Height",
      "type": "FloatProperty",
      "defaultValue": "0.8"
    },
    {
      "name": "undefined",
      "type": "MCCuboid",
      "extend": true,
      "properties": [
        {
          "name": "width",
          "defaultInput": "../../Width"
        },
        {
          "name": "height",
          "defaultInput": "../../Height"
        },
        {
          "name": "depth",
          "defaultInput": "../../Thickness"
        }
      ]
    }
  ]
});