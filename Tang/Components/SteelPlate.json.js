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
          "affects": [],
          "defaultInput": "../../undefined"
        }
      ]
    },
    {
      "name": "Thickness",
      "type": "FloatProperty",
      "affects": [],
      "defaultValue": "0.01"
    },
    {
      "name": "Width",
      "type": "FloatProperty",
      "affects": [],
      "defaultValue": "1.2"
    },
    {
      "name": "Height",
      "type": "FloatProperty",
      "affects": [],
      "defaultValue": "0.8"
    },
    {
      "name": "undefined",
      "type": "MCCuboid",
      "affects": [],
      "properties": [
        {
          "name": "width",
          "extend": true,
          "defaultInput": "../../Width"
        },
        {
          "name": "height",
          "extend": true,
          "defaultInput": "../../Height"
        },
        {
          "name": "depth",
          "extend": true,
          "defaultInput": "../../Thickness"
        }
      ]
    }
  ]
});