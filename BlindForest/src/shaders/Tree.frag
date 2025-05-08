#version 330 core

in vec3 fragPos;
in vec3 fragNormal;
out vec4 fragColor;

struct Light {
    vec3 lightColor;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light base;
    vec3 lightDirection;
};

uniform DirectionalLight directionalLight;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(-directionalLight.lightDirection);

    vec3 ambient = directionalLight.base.ambientIntensity * directionalLight.base.lightColor;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLight.base.diffuseIntensity * diff * directionalLight.base.lightColor;

    vec3 treeColor = vec3(0.2, 0.8, 0.2); // green
    vec3 result = (ambient + diffuse) * treeColor;

    fragColor = vec4(result, 1.0);
}