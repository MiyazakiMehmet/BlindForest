#pragma once

#include "Light.h"

class DirectionalLight : Light {
protected:
	glm::vec3 lightDirection;
public:
	DirectionalLight();
	DirectionalLight(glm::vec3 lightCol, float ambIntensity, float difIntensity,
		glm::vec3 lightDir);

	void UseLight(unsigned int lightColorLoc, unsigned int ambIntensityLoc, unsigned int difIntensityLoc,
				  unsigned int lightDirLoc);

	~DirectionalLight();
};