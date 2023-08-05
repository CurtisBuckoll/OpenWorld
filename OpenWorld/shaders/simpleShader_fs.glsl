#version 440 core

layout (binding = 0) uniform sampler2D diffTexture;
layout (binding = 1) uniform sampler2D specTexture;
layout (binding = 2) uniform sampler2D normalTexture;

layout (binding = 3) uniform sampler2D shadowMap;

in vec2 texCoord;
in vec3 fragPos;
in vec3 normal;
in vec3 tangentVec;
in vec4 fragPosLight;

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

float calcShadows(in vec4 lightFragPos) {
 vec3 projCoords = lightFragPos.xyz / lightFragPos.w;
 projCoords = projCoords * 0.5 + 0.5;
 float closestDepth = texture(shadowMap, projCoords.xy).r; 
 float currentDepth = projCoords.z;
 float bias = 0.005;

 return currentDepth - bias > closestDepth  ? 1.0 : 0.0;
}

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

   float shadow = calcShadows(fragPosLight);
   fragColor = vec4(ambient + (1.0 - shadow) * (diffuse + specular), 1.0);


   //fragColor = texture(normalTexture, texCoord);
   //fragColor = vec4(tangentVec, 1.0f);

   //fragColor = vec4(vec3(gl_FragCoord.z), 1.0);
}
