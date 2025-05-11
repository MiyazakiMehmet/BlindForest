#pragma once

#include "PointLight.h"

class SpotLight : PointLight {
protected:
	glm::vec3 direction;
	float edge, processedEdge;
public:
	SpotLight();
	SpotLight(glm::vec3 lightColor, float ambIntensity, float diffuseIntensity,
		glm::vec3 pos, float con, float lin, float exp,
		glm::vec3 lightDir, float edg);

	void UseLight(unsigned int colorLoc, unsigned int ambIntLoc, unsigned int difIntLoc,
		unsigned int positionLoc, unsigned int constantLoc, unsigned int linearLoc, unsigned int exponentLoc,
		unsigned int lightDirLoc, unsigned int edgeLoc);

	void UpdateSpotLightPosition(glm::vec3 pos);
	void UpdateSpotLightDirection(glm::vec3 dir);

	~SpotLight();
};