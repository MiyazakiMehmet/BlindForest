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
public:
	Shader();

	std::string ReadFile(std::string filePath);
	void CompileShader(std::string vertexShaderFile, std::string fragmentShaderFile);
	void UseShader();

	~Shader();
};

