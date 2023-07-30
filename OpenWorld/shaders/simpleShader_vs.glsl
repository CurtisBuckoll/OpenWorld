#version 440 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec3 tangent;

layout (std140, binding = 0) uniform ConstantBuffer
{
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 viewPos;
  uint bEnableNormalMaps;
};

out vec2 texCoord;
out vec3 fragPos;
out vec3 normal;
out vec3 tangentVec;

void main()
{
   fragPos = vec3(model * vec4(pos, 1.0));
   texCoord = uv;
   normal = mat3(transpose(inverse(model))) * norm;
   tangentVec = tangent;
   
   gl_Position = proj * view * vec4(fragPos, 1.0);
}
