#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec3 eyeVector;

uniform sampler2D env;
uniform vec2 resolution;
uniform vec3 viewPos;
uniform float ior;

void main() {
	// get screen coordinates
	vec2 uv = gl_FragCoord.xy / resolution;

	vec3 normal = Normal;
	// calculate refraction and add to the screen coordinates
	vec3 refracted = refract(eyeVector, normal, 1.0/ior);

	uv += (eyeVector - refracted).xy * 0.001 + vec2(0.05, 0);

	// uv += vec2(0.05, 0.05);
	
	// sample the background texture
	vec4 tex = texture2D(env, uv);

	vec4 output = tex;
	FragColor = vec4(output.rgb, 1.0);
}