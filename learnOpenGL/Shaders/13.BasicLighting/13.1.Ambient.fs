#version 330 core
out vec4 FragColor;

uniform vec3 baseColor;
uniform vec3 lightColor;
uniform float ambientStrength;

void main()
{
    vec3 ambient = lightColor * ambientStrength;
    vec3 result = ambient * baseColor;


    FragColor = vec4(result, 1);
}