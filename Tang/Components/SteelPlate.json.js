db.types.registerType({
  "name": "SteelPlate",
  "properties": [
    {
      "name": "renderGroup",
      "extend": true,
      "properties": [
        {
          "name": "1",
          "affects": [],
          "defaultInput": "../renderGroup/1"
        }
      ]
    },
    {
      "name": "Thickness",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "Width",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "Height",
      "affects": [],
      "defaultValue": "0"
    },
    {
      "name": "undefined",
      "affects": [],
      "properties": [
        {
          "name": "width",
          "extend": true,
          "defaultInput": "../undefined/width"
        },
        {
          "name": "height",
          "extend": true,
          "defaultInput": "../undefined/height"
        },
        {
          "name": "depth",
          "extend": true,
          "defaultInput": "../undefined/depth"
        }
      ]
    }
  ]
});