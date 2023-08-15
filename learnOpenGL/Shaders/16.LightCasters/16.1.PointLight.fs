#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float shininess;
};

struct Light {
vec3 position;

vec3 ambientStrength;
vec3 diffuseStrength;
vec3 specularStrength;
};

in vec3 Norm;
in vec3 FragPos;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;


void main()
{
    //ambient
    vec3 ambient = light.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));

    //diffuse
    vec3 lightDirection = light.position - FragPos;
    lightDirection = normalize(lightDirection);
    float diffStrength = max(dot(lightDirection, Norm), 0.0);
    vec3 diffuse = (diffStrength * vec3(texture(material.diffuseMap, TexCoord)) ) * light.diffuseStrength;

    //specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, Norm);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec *vec3(texture(material.specularMap, TexCoord))) * light.specularStrength;

    //output
    FragColor = vec4((ambient + diffuse + specular), 1.0);
}