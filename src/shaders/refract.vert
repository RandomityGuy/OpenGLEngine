#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

//layout (std140) uniform matrices
//{
uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 modelInverseTranspose;
//};

uniform vec3 viewPos;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;
out vec3 eyeVector;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(modelInverseTranspose) * aNormal;
    TexCoords = aTexCoords;
    eyeVector = normalize(FragPos - viewPos); 
}