#version 330 core

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D perlinNoise;

void main(){
	float n = texture(perlinNoise, texCoords).r;
	vec3 dirtColor = mix(vec3(0.2, 0.15, 0.1), vec3(0.4, 0.3, 0.2), n);

	fragColor = vec4(dirtColor, 1.0);
}