#version 440 core

out vec4 fragColor;

in vec3 texCoords;

layout(binding = 0) uniform samplerCube skybox;

void main()
{    
    fragColor = texture(skybox, texCoords);
}
