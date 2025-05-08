#pragma once
#include <GL/glew.h>

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "DirectionalLight.h"


class Shader
{
private:
	unsigned int shaderID;
	unsigned int vertexShader, fragmentShader;
	std::string vertexCode, fragmentCode;
	//Uniforms
	unsigned int modelLoc, viewLoc, projectionLoc,
		         lightColorLoc, ambientIntensityLoc, diffuseIntensityLoc, lightDirLoc;

public:
	Shader();

	std::string ReadFile(std::string filePath);
	void CompileShader(std::string vertexShaderFile, std::string fragmentShaderFile);
	void UseShader();
	unsigned int GetShaderID() { return shaderID; }

	//light
	void SetDirectionalLight(DirectionalLight directionalLight);
	//Getters for uniforms
	unsigned int GetModelLoc();
	unsigned int GetViewLoc();
	unsigned int GetProjectionLoc();
	unsigned int GetLightColorLoc();
	unsigned int GetAmbIntensityLoc();
	unsigned int GetDifIntensityLoc();
	unsigned int GetLightDirLoc();

	~Shader();
};

