#pragma once

#include <GL/glew.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

class Light {
protected:
	glm::vec3 lightColor;
	float ambientIntensity, diffuseIntensity;
public:
	Light();
	Light(glm::vec3 lightCol, float ambIntensity, float difIntensity);

	void UseLight(unsigned int lightColorLoc, unsigned int ambIntensityLoc, unsigned int difIntensityLoc);

	~Light();
};