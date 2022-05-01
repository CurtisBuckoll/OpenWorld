#version 440 core

layout(binding = 0) uniform sampler2D texture1;
layout(binding = 1) uniform sampler2D texture2;

in vec2 texCoord;
out vec4 FragColor;

void main()
{
    //FragColor = texture(ourTexture, texCoord);
    FragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.5);
}
