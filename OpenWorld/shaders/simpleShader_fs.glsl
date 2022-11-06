#version 440 core

layout(binding = 0) uniform sampler2D diffTexture;
layout(binding = 1) uniform sampler2D specTexture;

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
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor * texture(diffTexture, texCoord).rgb;

   vec3 norm = normalize(normal);
   vec3 dirToLight = normalize(-lightDir);
   float diff = max(dot(norm, dirToLight), 0.0);
   vec3 diffuse = diff * lightColor * texture(diffTexture, texCoord).rgb;

   float specularStrength = 0.8;
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(normalize(lightDir), norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor * texture(specTexture, texCoord).rgb;

   //fragColor = texture(texture1, texCoord);
   //fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);

   //fragColor = vec4((ambient + diffuse + specular) * texture(texture1, texCoord).xyz, 1.0);

   fragColor = vec4(ambient + diffuse + specular, 1.0);
}
