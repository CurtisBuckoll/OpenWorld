#version 440 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoord;

layout (std140, binding = 0) uniform ConstantBuffer
{
  mat4 model;
  mat4 view;
  mat4 proj;
};

out vec2 texCoord;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   texCoord = aTexCoord;
}
