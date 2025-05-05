#include "Shader.h"

Shader::Shader()
{
	shaderID = 0;
	vertexShader = 0;
	fragmentShader = 0;
}

std::string Shader::ReadFile(std::string filePath)
{
	std::ifstream file(filePath);
	std::stringstream buffer;

	if (!file.is_open()) {
		std::cout << "Failed to open file:" << filePath << std::endl;

		return "";
	}
	buffer << file.rdbuf(); //Read and store entire file into string stream
	file.close();

	return buffer.str(); //Transform to string
}

void Shader::CompileShader(std::string vertexShaderFile, std::string fragmentShaderFile)
{
	vertexCode = ReadFile(vertexShaderFile);
	fragmentCode = ReadFile(fragmentShaderFile);

	const char* c_VertexCode = vertexCode.c_str();
	const char* c_FragmentCode = fragmentCode.c_str();

	//Vertex Shader compile 
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &c_VertexCode, NULL);
	glCompileShader(vertexShader);

	//Error handling
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}


	//Fragment Shader compile
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &c_FragmentCode, NULL);
	glCompileShader(fragmentShader);

	//Error Handling
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
			infoLog << std::endl;
	}

	//Create Shader Program
	shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);
	glAttachShader(shaderID, fragmentShader);
	glLinkProgram(shaderID);

	//Error Handling
	glGetProgramiv(shaderID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderID, 512, NULL, infoLog);
	}

	//Delete shaders, since its linked to shader program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

Shader::~Shader()
{
	glDeleteProgram(shaderID);
}
