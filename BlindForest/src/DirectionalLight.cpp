#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	Light();
	lightDirection = glm::vec3(0.0f, -0.3f, 0.0f);
}

DirectionalLight::DirectionalLight(glm::vec3 lightCol, float ambIntensity, float difIntensity, glm::vec3 lightDir)
	: Light(lightCol, ambIntensity, difIntensity)
{
	lightDirection = lightDir;
}

void DirectionalLight::UseLight(unsigned int lightColorLoc, unsigned int ambIntensityLoc, unsigned int difIntensityLoc, unsigned int lightDirLoc)
{
	Light::UseLight(lightColorLoc, ambIntensityLoc, difIntensityLoc);
	glUniform3f(lightDirLoc, lightDirection.x, lightDirection.y, lightDirection.z);
}

DirectionalLight::~DirectionalLight()
{
}
