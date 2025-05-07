#pragma once
#include <GL/glew.h>

#include <string>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>


class Shader
{
private:
	unsigned int shaderID;
	unsigned int vertexShader, fragmentShader;
	std::string vertexCode, fragmentCode;
	//Uniforms
	unsigned int modelLoc, viewLoc, projectionLoc;

public:
	Shader();

	std::string ReadFile(std::string filePath);
	void CompileShader(std::string vertexShaderFile, std::string fragmentShaderFile);
	void UseShader();
	unsigned int GetShaderID() { return shaderID; }
	//Getters for uniforms
	unsigned int GetModelLoc();
	unsigned int GetViewLoc();
	unsigned int GetProjectionLoc();

	~Shader();
};

