#include "PointLight.h"

PointLight::PointLight()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 0.6f;
	linear = 0.3f;
	exponent = 0.1f;
}

PointLight::PointLight(glm::vec3 lightColor, float ambIntensity, float diffuseIntensity, glm::vec3 pos, float con, float lin, float exp)
	:Light(lightColor, ambIntensity, diffuseIntensity)
{
	position = pos;
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(unsigned int colorLoc, unsigned int ambIntLoc, unsigned int difIntLoc, unsigned int positionLoc, unsigned int constantLoc, unsigned int linearLoc, unsigned int exponentLoc)
{
	Light::UseLight(colorLoc, ambIntLoc, difIntLoc);
	glUniform3f(positionLoc, position.x, position.y, position.z);
	glUniform1f(constantLoc, constant);
	glUniform1f(linearLoc, linear);
	glUniform1f(exponentLoc, exponent);
}

PointLight::~PointLight()
{
}
