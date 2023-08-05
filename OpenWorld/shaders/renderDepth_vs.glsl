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

void main()
{
   gl_Position = proj * view * vec4(pos, 1.0);
}
