#version 330 core
out vec4 FragColor;

struct Material {
    bool hasAmbient;
    sampler2D ambient;
    bool hasDiffuse;
    sampler2D diffuse;
    bool hasSpecular;
    sampler2D specular;
    bool hasEmission;
    sampler2D emission;
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    vec3 emissionColor;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool enabled;
};

struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    bool enabled;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    bool enabled;
};

uniform Light dirLight;
uniform PointLight pointLights[4];
uniform SpotLight spotLight;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// function prototypes
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // phase 1: Directional lighting
    vec3 result = vec3(0);
    if (dirLight.enabled)
        result += CalcDirLight(dirLight, norm, viewDir);
    // phase 2: Point lights
    for(int i = 0; i < 4; i++)
    {
        if (pointLights[i].enabled)
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
    }
    // phase 3: Spot light
    if (spotLight.enabled)
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

    vec3 speccolor = (material.hasSpecular ? vec3(texture(material.specular, TexCoords)) : vec3(1)) * material.specularColor;

    float speclen = clamp(ceil(sqrt(speccolor.r * speccolor.r + speccolor.g * speccolor.g + speccolor.b * speccolor.b)), 0, 1);

    vec3 emissive = (1 - speclen) * (material.hasEmission ? vec3(texture(material.emission, TexCoords)) : vec3(1)) * material.emissionColor;

    result += emissive;

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * (material.hasAmbient ? vec3(texture(material.ambient, TexCoords)) : vec3(1)) * material.ambientColor * (material.hasDiffuse ? vec3(texture(material.diffuse, TexCoords)) : vec3(1)) * material.diffuseColor;
    vec3 diffuse = light.diffuse * diff * (material.hasDiffuse ? vec3(texture(material.diffuse, TexCoords)) : vec3(1)) * material.diffuseColor;
    vec3 specular = light.specular * spec * (material.hasSpecular ? vec3(texture(material.specular, TexCoords)) : vec3(1)) * material.specularColor;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * (material.hasAmbient ? vec3(texture(material.ambient, TexCoords)) : vec3(1)) * material.ambientColor * (material.hasDiffuse ? vec3(texture(material.diffuse, TexCoords)) : vec3(1)) * material.diffuseColor;
    vec3 diffuse = light.diffuse * diff * (material.hasDiffuse ? vec3(texture(material.diffuse, TexCoords)) : vec3(1)) * material.diffuseColor;
    vec3 specular = light.specular * spec * (material.hasSpecular ? vec3(texture(material.specular, TexCoords)) : vec3(1)) * material.specularColor;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * (material.hasAmbient ? vec3(texture(material.ambient, TexCoords)) : vec3(1)) * material.ambientColor * (material.hasDiffuse ? vec3(texture(material.diffuse, TexCoords)) : vec3(1)) * material.diffuseColor;
    vec3 diffuse = light.diffuse * diff * (material.hasDiffuse ? vec3(texture(material.diffuse, TexCoords)) : vec3(1)) * material.diffuseColor;
    vec3 specular = light.specular * spec * (material.hasSpecular ? vec3(texture(material.specular, TexCoords)) : vec3(1)) * material.specularColor;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}