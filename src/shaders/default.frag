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
    float reflectivity;
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

uniform samplerCube skybox;

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

// function prototypes
vec4 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float calcDirLightFactor(Light light, vec3 normal, vec3 viewDir);
float calcPointLightFactor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float calcSpotLightFactor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec4 calculateReflectiveColor(float refLightFactor, vec4 speccolor, vec3 viewDir, vec3 normal);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    float refLightFactor = 0;

    // phase 1: Directional lighting
    vec4 result = vec4(0);
    if (dirLight.enabled) {
        result += CalcDirLight(dirLight, norm, viewDir);
        refLightFactor += calcDirLightFactor(dirLight, norm, viewDir);
    }
    // phase 2: Point lights
    for(int i = 0; i < 4; i++)
    {
        if (pointLights[i].enabled) {
            result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
            refLightFactor += calcPointLightFactor(pointLights[i], norm, FragPos, viewDir);
        }
    }
    // phase 3: Spot light
    if (spotLight.enabled) {
        result += CalcSpotLight(spotLight, norm, FragPos, viewDir);
        refLightFactor += calcSpotLightFactor(spotLight, norm, FragPos, viewDir);
    }

    vec4 speccolor = (material.hasSpecular ? texture(material.specular, TexCoords) : vec4(1)) * vec4(material.specularColor, 1);

    float speclen = clamp(ceil(sqrt(speccolor.r * speccolor.r + speccolor.g * speccolor.g + speccolor.b * speccolor.b)), 0, 1);

    vec4 emissive = (1 - speclen) * (material.hasEmission ? texture(material.emission, TexCoords) : vec4(1)) * vec4(material.emissionColor, 1);

    if (result.a == 0)
        discard;

    result += emissive;

    result += calculateReflectiveColor(refLightFactor, speccolor, viewDir, Normal);

    FragColor = result;
}

vec4 calculateReflectiveColor(float refLightFactor, vec4 speccolor, vec3 viewDir, vec3 normal) {

    vec4 diffcolor = (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    speccolor = speccolor * refLightFactor;

    float refspeclen = sqrt(speccolor.r * speccolor.r + speccolor.g * speccolor.g + speccolor.b * speccolor.b) * (1 + material.reflectivity);

    vec3 R = reflect(-viewDir, normalize(normal));

    vec4 reflectColor = texture(skybox, R) * (refspeclen + (1 - diffcolor.a));

    reflectColor.a = diffcolor.a * material.reflectivity;

    return reflectColor;
}

float calcDirLightFactor(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    return diff;
}

float calcPointLightFactor(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    diff *= attenuation;
    return diff;
}

float calcSpotLightFactor(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    diff *= attenuation * intensity;
    return diff;
}

vec4 CalcDirLight(Light light, vec3 normal, vec3 viewDir) {
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = normalize(reflect(-lightDir, normal));
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec4 ambient = vec4(light.ambient, 1) * (material.hasAmbient ? texture(material.ambient, TexCoords) : vec4(1)) * vec4(material.ambientColor, 1) * (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    vec4 diffuse = vec4(light.diffuse, 1) * diff * (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    vec4 specular = vec4(light.specular, 1) * spec * (material.hasSpecular ? texture(material.specular, TexCoords) : vec4(1)) * vec4(material.specularColor, 1);

    vec3 R = reflect(-viewDir, normalize(Normal));

    vec4 specularTex = (material.hasSpecular ? texture(material.specular, TexCoords) : vec4(1)) * vec4(material.specularColor, 1);

    return (ambient + diffuse + specular);
}

vec4 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
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
    vec4 ambient = vec4(light.ambient, 1) * (material.hasAmbient ? texture(material.ambient, TexCoords) : vec4(1)) * vec4(material.ambientColor, 1) * (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    vec4 diffuse = vec4(light.diffuse, 1) * diff * (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    vec4 specular = vec4(light.specular, 1) * spec * (material.hasSpecular ? texture(material.specular, TexCoords) : vec4(1)) * vec4(material.specularColor, 1);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec4 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
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
    vec4 ambient = vec4(light.ambient, 1) * (material.hasAmbient ? texture(material.ambient, TexCoords) : vec4(1)) * vec4(material.ambientColor, 1) * (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    vec4 diffuse = vec4(light.diffuse, 1) * diff * (material.hasDiffuse ? texture(material.diffuse, TexCoords) : vec4(1)) * vec4(material.diffuseColor, 1);
    vec4 specular = vec4(light.specular, 1) * spec * (material.hasSpecular ? texture(material.specular, TexCoords) : vec4(1)) * vec4(material.specularColor, 1);
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}