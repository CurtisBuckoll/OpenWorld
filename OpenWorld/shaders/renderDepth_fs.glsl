#version 440 core

//layout (std140, binding = 1) uniform DirectionalLight
//{
//   vec3 lightDir;
//   vec3 lightColor;
//};

//vec3 lightDir = vec3(-1.0, -1.0, -1.0);
//vec3 lightColor = vec3(1.0, 1.0, 1.0);

out vec4 FragColor;

void main()
{
   // gl_FragDepth = gl_FragCoord.z;
   FragColor = vec4(vec3(gl_FragCoord.z), 1.0);
   //FragColor = vec4(0.3, 0.5, 0.9, 1.0);
}
