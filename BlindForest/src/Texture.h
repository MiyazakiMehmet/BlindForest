#pragma once

#include <GL/glew.h>

#include "stb_image.h"

#include <iostream>

class Texture {
private:
	unsigned int textureID;
	const char* texturePath;
	unsigned char* textureData;
	int width, height, nrChannels;
public:
	Texture();
	Texture(const char* texPath);

	bool LoadTexture();

	void CompileTexture();
	void UseTexture();

	~Texture();
};