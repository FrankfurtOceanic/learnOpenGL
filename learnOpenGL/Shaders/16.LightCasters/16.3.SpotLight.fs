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
vec3 direction;

float cutoff;

float constant;
float linear;
float quadratic;

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
    vec3 lightDirection =  normalize(light.position - FragPos);

    float cosTheta = dot(lightDirection, normalize(-light.direction));

    if(cosTheta > light.cutoff) //if within the cone created by the spot Light
    {
        //attenuation calculation
        float distance = length(FragPos - light.position);
        float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
        
        //ambient
        vec3 ambient = light.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));

        //diffuse
        vec3 norm = normalize(Norm);
        float diffStrength = max(dot(lightDirection, norm), 0.0);
        vec3 diffuse = (diffStrength * vec3(texture(material.diffuseMap, TexCoord)) ) * light.diffuseStrength;

        //specular
        vec3 viewDirection = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDirection, norm);
        float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
        vec3 specular = (spec *vec3(texture(material.specularMap, TexCoord))) * light.specularStrength;

        //output
        FragColor = vec4((ambient + diffuse + specular)* attenuation, 1.0);
        //FragColor = vec4(1.0);
    }

    else // use ambient light so scene isn’t black outside the spotlight.
    {
        FragColor = vec4(light.ambientStrength * vec3(texture(material.diffuseMap, TexCoord)), 1.0f);
    }
}