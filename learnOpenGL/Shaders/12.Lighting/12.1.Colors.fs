#version 330 core
out vec4 FragColor;

uniform vec3 baseColor;
uniform vec3 lightColor;

void main()
{
    FragColor = vec4(lightColor * baseColor, 1);
}