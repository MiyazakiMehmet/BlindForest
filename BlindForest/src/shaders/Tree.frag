#version 330 core

const int MAX_POINT_LIGHT = 3;
const int MAX_SPOT_LIGHT = 3;

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

struct PointLight{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material{
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;

uniform int pointLightCount;
uniform PointLight pointLight[MAX_POINT_LIGHT];

uniform int spotLightCount;
uniform SpotLight spotLight[MAX_SPOT_LIGHT];

uniform Material material;
uniform vec3 eyePos;

vec3 CalcLightByDirection(Light light, vec3 direction){
    vec3 normal = normalize(fragNormal);
    vec3 lightDirNormalized = normalize(-direction);

    vec3 ambient = light.ambientIntensity * light.lightColor;

    float diffuseFactor = max(dot(normal, lightDirNormalized), 0.0);
    vec3 diffuse = diffuseFactor * light.diffuseIntensity * light.lightColor;

    vec3 eyeToFrag = normalize(eyePos - fragPos);
    vec3 reflectedLight = reflect(lightDirNormalized, normal);

    vec3 specular = vec3(0.0);
    if(diffuseFactor > 0.0){
    float specularFactor = pow(max(dot(eyeToFrag, reflectedLight), 0.0), material.shininess); //Get angle between reflected light and eyeToFrag then power with shininess
    specular = specularFactor * material.specularIntensity * light.lightColor;
    }

    return (ambient + (diffuse + specular));
}

vec3 CalcDirectionalLight(){
    return CalcLightByDirection(directionalLight.base, directionalLight.lightDirection);
}

vec3 CalcPointLight(PointLight plight){
    vec3 direction = fragPos - plight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec3 color = CalcLightByDirection(plight.base, -direction);
    
    float attenuation = plight.exponent * distance * distance +
                            plight.linear * distance +
                            plight.constant;

    if (attenuation <= 0.0) {
           attenuation = 1.0; //prevent division by zero
       }

    return (color / attenuation);
}

vec3 CalcSpotLight(SpotLight slight){
    vec3 lightToFrag = normalize(fragPos - slight.base.position);

    float slFactor = dot(normalize(slight.direction), lightToFrag);

    if(slFactor > slight.edge){
        float intensity = (1.0f - (1.0f - slFactor)*(1.0f/(1.0f - slight.edge)));
        vec3 color = CalcPointLight(slight.base);
        return vec3(1.0, 1.0, 1.0) * intensity;
    }
    else{
        return vec3(0.0, 0.0, 0.0);
    }
}

vec3 CalcPointLights(){
    vec3 totalColor = vec3(0.0, 0.0, 0.0);

    for (int i = 0; i < pointLightCount; i++) {
        totalColor += CalcPointLight(pointLight[i]);
	}

    return totalColor;
}

vec3 CalcSpotLights(){
    vec3 totalColor = vec3(0.0, 0.0, 0.0);
    for(int i = 0; i < spotLightCount; i++){
        totalColor += CalcSpotLight(spotLight[i]);
    }

    return totalColor;
}

void main() {
    
    vec3 treeColor = vec3(0.0, 0.6, 0.0); // green
    vec3 finalColor = CalcDirectionalLight();
    finalColor += CalcPointLights();
    finalColor += CalcSpotLights();
    finalColor *= treeColor;


    fragColor = vec4(finalColor, 1.0);
}