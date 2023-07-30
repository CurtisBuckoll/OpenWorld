#version 440 core

layout (binding = 0) uniform sampler2D diffTexture;
layout (binding = 1) uniform sampler2D specTexture;
layout (binding = 2) uniform sampler2D normalTexture;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 tangentVec;

out vec4 fragColor;

layout (std140, binding = 0) uniform ConstantBuffer
{
  mat4 model;
  mat4 view;
  mat4 proj;
  vec3 viewPos;
  uint bEnableNormalMaps;
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
   vec4 diffTextureRGBA = texture(diffTexture, texCoord);
   if (diffTextureRGBA.a < 0.1) {
      // early out for alpha cutout
      discard;
   }

   vec3 mappedNormal = normal;

   if (bEnableNormalMaps == 1) {
      // TBN matrix
      vec3 N = normalize(normal);
      vec3 T = normalize(tangentVec);
      vec3 B = cross(N, T);
      mat3 TBN = mat3(T, B, N);
   
      mappedNormal = texture(normalTexture, texCoord).rgb * 2.0f - 1.0f;
      mappedNormal = TBN * normalize(mappedNormal);
   }

   // lighting calc
   float ambientStrength = 0.1;
   vec3 ambient = ambientStrength * lightColor * diffTextureRGBA.rgb;

   //vec3 surfNorm = normalize(normal);
   vec3 dirToLight = normalize(-lightDir);
   float diff = max(dot(mappedNormal, dirToLight), 0.0);
   vec3 diffuse = diff * lightColor * diffTextureRGBA.rgb;

   float specularStrength = 0.8;
   vec3 viewDir = normalize(viewPos - fragPos);
   vec3 reflectDir = reflect(normalize(lightDir), mappedNormal);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   vec3 specular = specularStrength * spec * lightColor * texture(specTexture, texCoord).rgb;

   //fragColor = texture(texture1, texCoord);
   //fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);

   //fragColor = vec4((ambient + diffuse + specular) * texture(texture1, texCoord).xyz, 1.0);

   fragColor = vec4(ambient + diffuse + specular, 1.0);


   //fragColor = texture(normalTexture, texCoord);
   //fragColor = vec4(tangentVec, 1.0f);
}
