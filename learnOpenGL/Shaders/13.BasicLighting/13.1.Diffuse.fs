#version 330 core
out vec4 FragColor;

in vec3 Norm;
in vec3 FragPos;

uniform vec3 baseColor;
uniform vec3 lightColor;
uniform float ambientStrength;
uniform float specularStrength;
uniform vec3 lightPosition;
uniform vec3 viewPos;


void main()
{
    //ambient
    vec3 ambient = lightColor * ambientStrength;

    //diffuse
    vec3 lightDirection = lightPosition - FragPos;
    lightDirection = normalize(lightDirection);
    float diffStrength = max(dot(lightDirection, Norm), 0.0);
    vec3 diffuse = diffStrength * lightColor;

    //specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, Norm);

    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), 32);
    vec3 specular = spec * specularStrength * lightColor;


 


    FragColor = vec4((ambient + diffuse + specular) * baseColor, 1);
}