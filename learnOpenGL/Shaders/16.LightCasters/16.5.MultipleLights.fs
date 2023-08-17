#version 330 core

out vec4 FragColor;

in vec2 TexCoord;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap;

    float shininess;
};
uniform Material material;

//Directional light
struct DirLight
{
    vec3 direction;
    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;
};
uniform DirLight dirLight;

//PointLight
struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

//SpotLight
struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutoff;
    float outerCutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambientStrength;
    vec3 diffuseStrength;
    vec3 specularStrength;
};
uniform SpotLight spot;


in vec3 Norm;
in vec3 FragPos;


uniform vec3 viewPos;

//functions
vec3 CalculateDirectionalLight(DirLight dLight, vec3 normal, vec3 viewDir);
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main()
{
    //calculate view direction 
    vec3 viewDirection = normalize(viewPos - FragPos);

    //unit length normal vector
    vec3 norm = normalize(Norm);
    
    // define an output color value
    vec3 output = vec3(0.0);

    //add directional Light
    output += CalculateDirectionalLight(dirLight, norm, viewDirection);

    //add point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
    {
        output += CalculatePointLight(pointLights[i], norm, viewDirection, FragPos);
    }

    // and add others lights as well (like spotlights)
    output += CalculateSpotLight(spot, norm, viewDirection, FragPos);

    FragColor = vec4(output, 1.0);
}

vec3 CalculateDirectionalLight(DirLight dLight, vec3 normal, vec3 viewDir)
{
    vec3 ambient = dLight.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));

    //diffuse
    vec3 lightDirection = normalize(-dLight.direction);
    float diffStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = (diffStrength * vec3(texture(material.diffuseMap, TexCoord)) ) * dLight.diffuseStrength;

    //specular
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec *vec3(texture(material.specularMap, TexCoord))) * dLight.specularStrength;

    return ambient + diffuse + specular;
}

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    //attenuation calculation

    float distance = length(fragPos - light.position);
    float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
    //ambient
    vec3 ambient = light.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));

    //diffuse
    vec3 lightDirection = light.position - fragPos;
    lightDirection = normalize(lightDirection);
    float diffStrength = max(dot(lightDirection, normal), 0.0);
    vec3 diffuse = (diffStrength * vec3(texture(material.diffuseMap, TexCoord)) ) * light.diffuseStrength;

    //specular

    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec *vec3(texture(material.specularMap, TexCoord))) * light.specularStrength;

    //output
    return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
    vec3 lightDirection =  normalize(light.position - FragPos);

    float cosTheta = dot(lightDirection, normalize(-light.direction));

    //adding smooth edges
    float epsilon = light.cutoff - light.outerCutoff;
    float intensity = clamp((cosTheta - light.outerCutoff) / epsilon, 0.0, 1.0);

        
    //attenuation calculation
    float distance = length(FragPos - light.position);
    float attenuation = 1 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
        
    //ambient
    vec3 ambient = light.ambientStrength * vec3(texture(material.diffuseMap, TexCoord));

    //diffuse
    vec3 norm = normalize(Norm);
    float diffStrength = max(dot(lightDirection, norm), 0.0);
    vec3 diffuse = (diffStrength * vec3(texture(material.diffuseMap, TexCoord)) ) * light.diffuseStrength;
    diffuse *= intensity;

    //specular
    vec3 viewDirection = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDirection, norm);
    float spec = pow(max(dot(viewDirection, reflectDir), 0.0), material.shininess);
    vec3 specular = (spec *vec3(texture(material.specularMap, TexCoord))) * light.specularStrength;
    specular *= intensity;

    //output
    return (ambient + diffuse + specular)* attenuation;
}