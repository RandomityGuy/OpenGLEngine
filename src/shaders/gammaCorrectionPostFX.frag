#version 330 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D screenTexture;
uniform float gamma;

void main()
{ 
    vec4 screenColor = texture(screenTexture, TexCoords);
    screenColor.rgb = pow(screenColor.rgb, vec3(1.0/gamma));
    FragColor = screenColor;
}