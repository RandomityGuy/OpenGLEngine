#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform sampler2D refractTexture;

void main()
{ 
    vec4 screenColor = texture(screenTexture, TexCoords);
    vec4 refractColor = texture(refractTexture, TexCoords);
    if (refractColor.a == 0)
        FragColor = screenColor;
    else
        FragColor = refractColor;
}