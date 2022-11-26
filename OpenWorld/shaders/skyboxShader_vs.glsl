#version 440 core

layout (location = 0) in vec3 pos;

layout (std140, binding = 0) uniform ConstantBuffer
{
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 viewPos;
};

out vec3 texCoords;

void main()
{
    texCoords = pos;
    mat4 rotOnlyView = mat4(mat3(view));  
    vec4 projPos = proj * rotOnlyView * model * vec4(pos, 1.0);
    // set to xyww so that z at infinity. we render skybox last and treat it as if it is projected at inf
    // note that depth test must then be GL_LEQUAL and not GL_LESS
    gl_Position = projPos.xyww;
}
