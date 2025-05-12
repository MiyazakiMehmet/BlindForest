#include "Light.h"

Light::Light()
{
	lightColor = glm::vec3(1.0, 1.0, 1.0);
	ambientIntensity = 0.3f;
	diffuseIntensity = 0.3f;
}

Light::Light(glm::vec3 lightCol, float ambIntensity, float difIntensity)
{
	lightColor = lightCol;
	ambientIntensity = ambIntensity;
	diffuseIntensity = difIntensity;
}
//h
void Light::UseLight(unsigned int lightColorLoc, unsigned int ambIntensityLoc, unsigned int difIntensityLoc)
{
	glUniform3f(lightColorLoc, lightColor.x, lightColor.y, lightColor.z);
	glUniform1f(ambIntensityLoc, ambientIntensity);
	glUniform1f(difIntensityLoc, diffuseIntensity);
}

Light::~Light()
{
}
