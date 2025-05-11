#pragma once
#include <GL/glew.h>

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "GlobalValues.h"

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"


class Shader
{
private:
	unsigned int shaderID;
	unsigned int vertexShader, fragmentShader;
	std::string vertexCode, fragmentCode;
	//Uniforms
	unsigned int modelLoc, viewLoc, projectionLoc,
		  		lightColorLoc, ambientIntensityLoc, diffuseIntensityLoc, lightDirLoc,
		 		specularIntensityLoc, shininessLoc, eyePosLoc,
		 		pointLightCountLoc, spotLightCountLoc;

	struct PointLightUniform {
		unsigned int colorLoc;
		unsigned int ambientIntensityLoc;
		unsigned int diffuseIntensityLoc;

		unsigned int positionLoc;
		unsigned int constantLoc;
		unsigned int linearLoc;
		unsigned int exponentLoc;
	};

	PointLightUniform pointLights[MAX_POINT_LIGHT];

	struct SpotLightUniform {
		unsigned int colorLoc;
		unsigned int ambientIntensityLoc;
		unsigned int diffuseIntensityLoc;

		unsigned int positionLoc;
		unsigned int constantLoc;
		unsigned int linearLoc;
		unsigned int exponentLoc;
		unsigned int lightDirLoc;
		unsigned int edgeLoc;
	};

	SpotLightUniform spotLights[MAX_SPOT_LIGHT];

public:
	Shader();

	std::string ReadFile(std::string filePath);
	void CompileShader(std::string vertexShaderFile, std::string fragmentShaderFile);
	void UseShader();
	unsigned int GetShaderID() { return shaderID; }

	//light
	void SetDirectionalLight(DirectionalLight directionalLight);
	void SetPointLight(PointLight* pointLight, int pointLightCount);
	void SetSpotLight(SpotLight* spotLight, int spotLightCount);
	void SetMaterial(Material material);
	//Getters for uniforms
	unsigned int GetModelLoc();
	unsigned int GetViewLoc();
	unsigned int GetProjectionLoc();
	unsigned int GetEyePosLoc();

	~Shader();
};

