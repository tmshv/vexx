db.types.registerType({
  "name": "StandardSurface",
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
      "name": "ambient",
      "type": "ColourProperty",
      "defaultValue": "0.309804 0.309804 0.309804 1"
    },
    {
      "name": "diffuse",
      "type": "ColourProperty",
      "defaultValue": "0.513726 0.513726 0.513726 1",
      "affects": [
        "runtimeShader"
      ]
    },
    {
      "name": "Vert",
      "type": "GCVertexShaderComponent",
      "defaultValue": "attribute vec3 vertex;\nattribute vec3 normals;\nvarying vec3 normal;\n\nvoid main(void)\n  {\n  normal = gl_NormalMatrix * normals;\n  gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * vec4(vertex,1.0);\n  }\n"
    },
    {
      "name": "Frag",
      "type": "GCFragmentShaderComponent",
      "defaultValue": "varying vec3 normal;\nuniform vec4 ambient;\nuniform vec4 diffuse;\n\nvoid main(void)\n  {\n  vec3 lightDir = vec3(-0.7, -0.8, -1.0);\n  float dot = max(0.0, dot(normal, -lightDir));\n  gl_FragColor = ambient + dot * diffuse;\n  }\n"
    }
  ]
});
