#include "SpotLight.h"

SpotLight::SpotLight()
{
	direction = glm::vec3(0.0f, 0.0f, 1.0f);
	edge = 30.0f;
	processedEdge = glm::cos(glm::radians(edge));
}

SpotLight::SpotLight(glm::vec3 lightColor, float ambIntensity, float diffuseIntensity, glm::vec3 pos, float con, float lin, float exp, glm::vec3 lightDir, float edg)
	:PointLight(lightColor, ambIntensity, diffuseIntensity, pos, con, lin, exp)
{
	direction = lightDir;
	edge = edg;
	processedEdge = glm::cos(glm::radians(edge));
}

void SpotLight::UseLight(unsigned int colorLoc, unsigned int ambIntLoc, unsigned int difIntLoc, unsigned int positionLoc, unsigned int constantLoc, unsigned int linearLoc, unsigned int exponentLoc, unsigned int lightDirLoc, unsigned int edgeLoc)
{
	PointLight::UseLight(colorLoc, ambIntLoc, difIntLoc, positionLoc, constantLoc, linearLoc, exponentLoc);
	glUniform3f(lightDirLoc, direction.x, direction.y, direction.z);
	glUniform1f(edgeLoc, processedEdge);
}

void SpotLight::UpdateSpotLightPosition(glm::vec3 pos)
{
	position = pos;
}

void SpotLight::UpdateSpotLightDirection(glm::vec3 dir)
{
	direction = dir;
}

SpotLight::~SpotLight()
{
}
