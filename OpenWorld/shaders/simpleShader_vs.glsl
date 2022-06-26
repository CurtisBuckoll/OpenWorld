#version 440 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColour;
layout(location = 2) in vec2 aTexCoord;

layout (std140, binding = 0) uniform ConstantBuffer
{
    mat4 proj;
    mat4 view;
};

out vec2 texCoord;

void main()
{
   gl_Position = vec4(aPos, 1.0);
   texCoord = aTexCoord;
}
