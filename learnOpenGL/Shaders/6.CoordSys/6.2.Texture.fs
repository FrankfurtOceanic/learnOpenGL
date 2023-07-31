#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D ourTexture;
uniform sampler2D texture2;

uniform float mixPercent;

void main()
{
    FragColor = mix(texture(ourTexture, TexCoord), texture(texture2, TexCoord), mixPercent);
}