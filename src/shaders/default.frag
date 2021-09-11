#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
}; 
  
uniform Material material;

struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;  

uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;  
in vec2 TexCoords;


void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords)); 

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    vec3 speccolor = vec3(texture(material.specular, TexCoords));

    vec3 specular = spec * light.specular * vec3(speccolor);

    float speclen = clamp(ceil(sqrt(speccolor.r * speccolor.r + speccolor.g * speccolor.g + speccolor.b * speccolor.b)), 0, 1);
    
    vec3 emissive = (1 - speclen) * vec3(texture(material.emission, TexCoords));

    vec3 result = (ambient + diffuse + specular + emissive);
    FragColor = vec4(result, 1.0);
}  