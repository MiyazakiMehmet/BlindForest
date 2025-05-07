#version 330 core

in vec2 texCoords;
out vec4 fragColor;

uniform sampler2D perlinNoise;

float fbm(vec2 p) {
    float total = 0.0;
    float amplitude = 0.3;
    float frequency = 1.0;

    for (int i = 0; i < 5; i++) {
        float n = texture(perlinNoise, p * frequency).r;
        total += n * amplitude;
        frequency *= 2.0;
        amplitude *= 0.5;
    }

    return total;
}

void main(){
	//float n = texture(perlinNoise, texCoords).r;
    float n = fbm(texCoords);
	
    vec3 dirtColor = mix(vec3(0.1, 0.03, 0.03), vec3(0.0, 0.16, 0.0),n);

	fragColor = vec4(dirtColor, 1.0);
}