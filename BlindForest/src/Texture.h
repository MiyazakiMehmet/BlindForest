#pragma once

#include <GL/glew.h>

#include "stb_image.h"

class Texture {
private:
	unsigned int textureID;
	const char* texturePath;
	int width, height, nrChannels;
public:
	Texture();
	Texture(const char* texPath);

	void CompileTexture();
	void UseShader();

	~Texture();
};