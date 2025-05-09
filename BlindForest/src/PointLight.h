#pragma once

#include "Light.h"

class PointLight : Light {
protected:
	glm::vec3 position;
	float constant, linear, exponent;
public:
	PointLight();
	PointLight(glm::vec3 lightColor, float ambIntensity, float diffuseIntensity,
			   glm::vec3 pos, float con, float lin, float exp);

	void UseLight(unsigned int colorLoc, unsigned int ambIntLoc, unsigned int difIntLoc,
				  unsigned int positionLoc, unsigned int constantLoc, unsigned int linearLoc, unsigned int exponentLoc);

	~PointLight();
};