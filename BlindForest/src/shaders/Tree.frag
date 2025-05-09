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

struct Material{
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePos;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(-directionalLight.lightDirection);

    vec3 ambient = directionalLight.base.ambientIntensity * directionalLight.base.lightColor;
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = directionalLight.base.diffuseIntensity * diff * directionalLight.base.lightColor;

    vec3 eyeToFrag = normalize(eyePos - fragPos);
    vec3 reflectedLight = reflect(lightDir, normal);
    vec3 specular = vec3(0.0);
    if(diff > 0.0){
      float specularFactor = pow(max(dot(eyeToFrag, reflectedLight), 0.0), material.shininess); //Get angle between reflected light and eyeToFrag then power with shininess
      specular = specularFactor * material.specularIntensity * directionalLight.base.lightColor;
    }
    vec3 treeColor = vec3(0.0, 0.6, 0.0); // green
    vec3 finalColor = (ambient + diffuse + specular) * treeColor;


    fragColor = vec4(finalColor, 1.0);
}