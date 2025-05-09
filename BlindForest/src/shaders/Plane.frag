#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 fragNormal;
out vec4 fragColor;

struct Light{
    vec3 lightColor;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight{
    Light base;
    vec3 lightDirection;
};

struct Material{
    float specularIntensity;
    float shininess;
};

uniform sampler2D perlinNoise;
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePos;

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
	
    vec3 dirtColor = mix(vec3(0.8, 0.03, 0.03), vec3(0.0, 0.66, 0.0),n);

    vec3 normal = normalize(fragNormal);
    vec3 lightDirNormalized = normalize(-directionalLight.lightDirection);

    vec3 ambient = directionalLight.base.ambientIntensity * directionalLight.base.lightColor;

    float diffuseFactor = max(dot(normal, lightDirNormalized), 0.0);
    vec3 diffuse = diffuseFactor * directionalLight.base.diffuseIntensity * directionalLight.base.lightColor;

    vec3 eyeToFrag = normalize(eyePos - fragPos);
    vec3 reflectedLight = reflect(lightDirNormalized, normal);

    vec3 specular = vec3(0.0);
    if(diffuseFactor > 0.0){
    float specularFactor = pow(max(dot(eyeToFrag, reflectedLight), 0.0), material.shininess); //Get angle between reflected light and eyeToFrag then power with shininess
    specular = specularFactor * material.specularIntensity * directionalLight.base.lightColor;
    }

    vec3 baseColor = vec3(0.6, 0.4, 0.2); // brighter brown
    vec3 finalColor = (ambient + diffuse) * vec3(0.0, 0.0, 1.0);
    finalColor += specular;

	fragColor = vec4(finalColor, 1.0);
}