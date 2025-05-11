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

	//Setting uniform locations
	modelLoc = glGetUniformLocation(shaderID, "model");
	viewLoc = glGetUniformLocation(shaderID, "view");
	projectionLoc = glGetUniformLocation(shaderID, "projection");
	lightColorLoc = glGetUniformLocation(shaderID, "directionalLight.base.lightColor");
	ambientIntensityLoc = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
	diffuseIntensityLoc = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
	lightDirLoc = glGetUniformLocation(shaderID, "directionalLight.lightDirection");
	specularIntensityLoc = glGetUniformLocation(shaderID, "material.specularIntensity");
	shininessLoc = glGetUniformLocation(shaderID, "material.shininess");
	eyePosLoc = glGetUniformLocation(shaderID, "eyePos");
	
	//Point Light
	pointLightCountLoc = glGetUniformLocation(shaderID, "pointLightCount");

	for (size_t i = 0; i < MAX_POINT_LIGHT; i++) {
		char locBuff[100] = { '\0' };

		// base.lightColor
		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].base.lightColor", i);
		pointLights[i].colorLoc =
			glGetUniformLocation(shaderID, locBuff);

		// base.ambientIntensity
		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].base.ambientIntensity", i);
		pointLights[i].ambientIntensityLoc =
			glGetUniformLocation(shaderID, locBuff);

		// base.diffuseIntensity
		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].base.diffuseIntensity", i);
		pointLights[i].diffuseIntensityLoc =
			glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].position", i); //That func will store the string that we specified (%d = 0,1,2...)
		pointLights[i].positionLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].constant", i); //That func will store the string that we specified (%d = 0,1,2...)
		pointLights[i].constantLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].linear", i); //That func will store the string that we specified (%d = 0,1,2...)
		pointLights[i].linearLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "pointLight[%d].exponent", i); //That func will store the string that we specified (%d = 0,1,2...)
		pointLights[i].exponentLoc = glGetUniformLocation(shaderID, locBuff);
	}

	//SpotLight
	spotLightCountLoc = glGetUniformLocation(shaderID, "spotLightCount");

	for (size_t i = 0; i < MAX_SPOT_LIGHT; i++) {
		char locBuff[100] = { '\0' };

		// base.lightColor
		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.base.lightColor", i);
		spotLights[i].colorLoc =
			glGetUniformLocation(shaderID, locBuff);

		// base.ambientIntensity
		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.ambientIntensity", i);
		spotLights[i].ambientIntensityLoc =
			glGetUniformLocation(shaderID, locBuff);

		// base.diffuseIntensity
		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.diffuseIntensity", i);
		spotLights[i].diffuseIntensityLoc =
			glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].base.position", i); //That func will store the string that we specified (%d = 0,1,2...)
		spotLights[i].positionLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].constant", i); //That func will store the string that we specified (%d = 0,1,2...)
		spotLights[i].constantLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].linear", i); //That func will store the string that we specified (%d = 0,1,2...)
		spotLights[i].linearLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].exponent", i); //That func will store the string that we specified (%d = 0,1,2...)
		spotLights[i].exponentLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].direction", i); //That func will store the string that we specified (%d = 0,1,2...)
		spotLights[i].lightDirLoc = glGetUniformLocation(shaderID, locBuff);

		snprintf(locBuff, sizeof(locBuff), "spotLight[%d].edge", i); //That func will store the string that we specified (%d = 0,1,2...)
		spotLights[i].edgeLoc = glGetUniformLocation(shaderID, locBuff);
	}
}

void Shader::UseShader()
{
	glUseProgram(shaderID);
}

void Shader::SetDirectionalLight(DirectionalLight directionalLight)
{
	directionalLight.UseLight(lightColorLoc, ambientIntensityLoc, diffuseIntensityLoc, lightDirLoc);
}

void Shader::SetPointLight(PointLight* pointLight, int pointLightCount)
{
	if (pointLightCount > MAX_POINT_LIGHT) {
		pointLightCount = MAX_POINT_LIGHT;
	}

	glUniform1i(pointLightCountLoc, pointLightCount);


	for (int i = 0; i < pointLightCount; i++) {
		pointLight[i].UseLight(pointLights[i].colorLoc, pointLights[i].ambientIntensityLoc, pointLights[i].diffuseIntensityLoc,
			pointLights[i].positionLoc, pointLights[i].constantLoc, pointLights[i].linearLoc, pointLights[i].exponentLoc);
	}
}

void Shader::SetSpotLight(SpotLight* spotLight, int spotLightCount)
{
	if (spotLightCount > MAX_SPOT_LIGHT) {
		spotLightCount = MAX_SPOT_LIGHT;
	}

	glUniform1i(spotLightCountLoc, spotLightCount);


	for (int i = 0; i < spotLightCount; i++) {
		spotLight[i].UseLight(spotLights[i].colorLoc, spotLights[i].ambientIntensityLoc, spotLights[i].diffuseIntensityLoc,
			spotLights[i].positionLoc, spotLights[i].constantLoc, spotLights[i].linearLoc, spotLights[i].exponentLoc,
			spotLights[i].lightDirLoc, spotLights[i].edgeLoc);
	}
}

void Shader::SetMaterial(Material material)
{
	material.UseMaterial(specularIntensityLoc, shininessLoc);
}

unsigned int Shader::GetModelLoc()
{
	return modelLoc;
}

unsigned int Shader::GetViewLoc()
{
	return viewLoc;
}

unsigned int Shader::GetProjectionLoc()
{
	return projectionLoc;
}

unsigned int Shader::GetEyePosLoc()
{
	return eyePosLoc;
}

Shader::~Shader()
{
	glDeleteProgram(shaderID);
}
