db.addType({
  "name": "FlatSurface",
  "parent": "GCShader",
  "properties": [
    {
      "name": "components",
      "extend": true,
      "properties": [
        {
          "name": "1",
          "type": "GCShaderComponentPointer",
          "defaultInput": "../../Vert"
        },
        {
          "name": "2",
          "type": "GCShaderComponentPointer",
          "defaultInput": "../../Frag"
        }
      ]
    },
    {
      "name": "texture",
      "type": "GCTexturePointer",
      "affects": [
        "runtimeShader"
      ]
    },
    {
      "name": "Vert",
      "type": "GCVertexShaderComponent",
      "defaultValue": "attribute vec3 vertex;\nattribute vec3 normals;\nattribute vec2 uvSet1;\nvarying vec2 uv;\n\nvoid main(void)\n  {\n  uv = uvSet1;\n  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(vertex,1.0);\n  }\n"
    },
    {
      "name": "Frag",
      "type": "GCFragmentShaderComponent",
      "defaultValue": "varying vec2 uv;\nuniform vec4 ambient;\nuniform vec4 diffuse;\nuniform sampler2D texture;\n\nvoid main(void)\n  {\n  gl_FragColor = texture2D(texture, uv);\n  }\n"
    }
  ]
});