#pragma once

#include <GL/glew.h>

class Material {
private:
	float specularIntensity;
	float shininess;
public:
	Material();
	Material(float specIntensity, float shine);
	void UseMaterial(unsigned int specularIntensityLoc, unsigned int shininessLoc);

	~Material();
};