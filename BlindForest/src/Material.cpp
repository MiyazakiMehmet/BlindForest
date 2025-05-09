#include "Material.h"

Material::Material()
{
	specularIntensity = 0.3f;
	shininess = 4.0f;
}

Material::Material(float specIntensity, float shine)
{
	specularIntensity = specIntensity;
	shininess = shine;
}

void Material::UseMaterial(unsigned int specularIntensityLoc, unsigned int shininessLoc)
{
	glUniform1f(specularIntensityLoc, specularIntensity);
	glUniform1f(shininessLoc, shininess);
}

Material::~Material()
{
}
