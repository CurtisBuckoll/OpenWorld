#version 440 core

layout (binding = 0) uniform sampler2D diffTexture;
layout (binding = 1) uniform sampler2D specTexture;
layout (binding = 2) uniform samplerCube skybox;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

layout (std140, binding = 0) uniform ConstantBuffer
{
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 viewPos;
};

//layout (std140, binding = 1) uniform DirectionalLight
//{
//   vec3 lightDir;
//   vec3 lightColor;
//};

vec3 lightDir = vec3(-1.0, -1.0, -1.0);
vec3 lightColor = vec3(1.0, 1.0, 1.0);

void main()
{   
   // reflection
   vec3 i = normalize(fragPos - viewPos);
   vec3 r = reflect(i, normalize(normal));
   fragColor = vec4(texture(skybox, r).rgb, 1.0);


   // refraction
   //float ratio = 1.00 / 1.52;
   //vec3 i = normalize(fragPos - viewPos);
   //vec3 r = refract(i, normalize(normal), ratio);
   //fragColor = vec4(texture(skybox, r).rgb, 1.0);
}
